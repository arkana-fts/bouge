# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

###
# WORKS:
#  * vertex coordinates
#  * vertex and face normals (aka smooth and flat shading)
#  * splitting quads into triangles
# TODO:
#  * uv texcos
#  * influences
#  * export skeleton
#  * export materials
#  * other vertex attributes?
###

import os
import time
import shutil

import bpy
import mathutils
import math

from . import common

def save_mesh(filepath, objects, scene,
              preprocess_matrix,          # Yes
              use_apply_modifiers,        # Yes
              use_normalize_influences,   # Yes
              only_four_influences):      # Yes
    '''
    Basic write function. The context and options must be already set
    This can be accessed externaly
    eg.
    write( 'c:\\test\\foobar.obj', Blender.Object.GetSelected() ) # Using default options.
    '''

    mesh = common.BougeMesh(os.path.splitext(os.path.basename(filepath))[0])

    armatures_needed = dict()
    materials_needed = dict()
    mesh_material_assossiations = dict()

    for obj in objects:
        if obj.type == 'ARMATURE':
            armatures_needed[obj.name] = obj
            continue

        if obj.type != 'MESH':
            continue

        if obj.find_armature():
            armatures_needed[obj.find_armature().name] = obj.find_armature()

        ob_mat = obj.matrix_world

        try:
            # 2.58
            createmesh = obj.to_mesh
        except AttributeError:
            # 2.56
            createmesh = obj.create_mesh

        me = createmesh(scene, use_apply_modifiers, 'PREVIEW')
        me.transform(preprocess_matrix * ob_mat)

        # We could split the object into submeshes by-material.
        # For now, it's easier to just ask the user to do that...
        # Still export all of the materials, as it might be the user did well by
        # choosing just one material for all the vertices but allowing more "skins".
        if len(me.materials) > 1:
            print('WARNING: The mesh {} uses more than one material.'.format(me.name))
            #continue

        if obj.data.name == obj.name:
            subme = mesh.newsubmesh(obj.data.name)
        else:
            subme = mesh.newsubmesh(obj.name + " " + obj.data.name)

        materials_needed.update(me.materials)

        # Keep that in mind for creating material sets.
        if len(me.materials) > 0:
            mesh_material_assossiations[subme.name] = [mat.name for mat in me.materials if mat]

        for f in me.faces:
            # Triangles can be added as-is
            if len(f.vertices) == 3:
                subme.addFace(use_normalize_influences, only_four_influences, obj, me, None if f.use_smooth else f.normal, f, 0, 1, 2)

            # Try to make an educated guess splitting up quads...
            elif len(f.vertices) == 4:
                def triNormal(v1, v2, v3):
                    return mathutils.geometry.normal(me.vertices[f.vertices[v1]].co, me.vertices[f.vertices[v2]].co, me.vertices[f.vertices[v3]].co)

                ideal_normal = f.normal
                # Try the first possibility:
                f1a = 0, 1, 2
                f1b = 0, 2, 3
                n1 = (triNormal(*f1a) + triNormal(*f1b)).normalized()
                # And the second possibility:
                f2a = 0, 1, 3
                f2b = 1, 2, 3
                n2 = (triNormal(*f2a) + triNormal(*f2b)).normalized()

                # Now choose the split whose sum of normals is closes to the quad's normal
                if ideal_normal.dot(n1) > ideal_normal.dot(n2):
                    subme.addFace(use_normalize_influences, only_four_influences, obj, me, None if f.use_smooth else f.normal, f, *f1a)
                    subme.addFace(use_normalize_influences, only_four_influences, obj, me, None if f.use_smooth else f.normal, f, *f1b)
                else:
                    subme.addFace(use_normalize_influences, only_four_influences, obj, me, None if f.use_smooth else f.normal, f, *f2a)
                    subme.addFace(use_normalize_influences, only_four_influences, obj, me, None if f.use_smooth else f.normal, f, *f2b)

    file = open(filepath, "w", encoding="utf8", newline="\n")
    file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))
    mesh.writeXML(file)
    file.close()

    return armatures_needed, materials_needed, mesh_material_assossiations

def __rootBone(bone):
    if not bone.parent:
        return bone
    else:
        return __rootBone(bone.parent)

def make_BougeBone(bone, preprocess_matrix):
    # TODO: test if that preprocess_matrix actually works.
    if bone.parent:
        relativePos = mathutils.Vector((0, bone.parent.length, 0))

        if not bone.use_connect:
            relativePos += bone.head
    else:
        relativePos = bone.head * preprocess_matrix

    relativeRot = bone.matrix.to_quaternion()

    # We only need to rotate root bones by that, all others will follow!
    if not bone.parent:
        try:
            # Works since 2.58 (and 2.57?)
            relativeRot.rotate(preprocess_matrix)
        except TypeError:
            # Workaround for 2.56
            relativeRotM = relativeRot.to_matrix()
            relativeRotM.rotate(preprocess_matrix)
            relativeRot = relativeRotM.to_quaternion()

    return common.BougeBone(bone.name, bone.length, relativePos, relativeRot)

def __makeAndAttachParentRecursive(bone, allbones, rootbones, preprocess_matrix):
    bbone = make_BougeBone(bone, preprocess_matrix)
    allbones[bone.name] = bbone

    if not bone.parent:
        rootbones[bbone.name] = bbone
        return bbone

    if bone.parent.name in allbones:
        # If the parent already exists, add this bone to its children and done!
        allbones[bone.parent.name].giveChild(bbone)
    else:
        # The parent doesn't exist yet, create it!
        parent = __makeAndAttachParentRecursive(bone.parent, allbones)
        parent.giveChild(bbone)

    return bbone

def save_armature(filepath, arm,
                  preprocess_matrix   # No (TODO)
                 ):
    allbones = dict()
    rootbones = dict()

    # Unfortunately, at the time of writing this, bone.children has always
    # been empty. bone.parent worked though, so we build the bone hierarchy
    # from bottom-up, which is slightly more complicated. But works :)
    for bone in arm.data.bones.values():
        __makeAndAttachParentRecursive(bone, allbones, rootbones, preprocess_matrix)

    #print("all:")
    #print(allbones)
    #print("roots:")
    #print(rootbones)

    skel = common.BougeSkel(os.path.splitext(os.path.basename(filepath))[0])

    for rootbone in rootbones.values():
        skel.giveRootBone(rootbone)

    file = open(filepath, "w", encoding="utf8", newline="\n")
    file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))
    skel.writeXML(file)
    file.close()

def __col2str(col, alpha = None):
    return '{:g} {:g} {:g} {:g}'.format(col.r, col.g, col.b, alpha or 1)

def save_materials(filepath, mats):
    file = open(filepath, "w", encoding="utf8", newline="\n")
    file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))

    for mat in mats:
        bmat = common.BougeMaterial(mat.name)

        # Default phong light model values.
        dc = mat.diffuse_color
        sc = mat.specular_color
        di = mat.diffuse_intensity
        si = mat.specular_intensity
        #si = mat.specular_hardness
        bmat.setprop('uMaterialAmbient', __col2str(mathutils.Color((0.2, 0.2, 0.2))))
        bmat.setprop('uMaterialDiffuse', __col2str(mathutils.Color((dc.r * di, dc.g * di, dc.b * di))))
        #bmat.setprop('specular', __col2str(mathutils.Color((sc.r * si, sc.g * si, sc.b * si))))
        #bmat.setprop('shininess', str(mat.specular_hardness))
        bmat.setprop('uMaterialSpecular', __col2str(mathutils.Color((sc.r * si, sc.g * si, sc.b * si)), mat.specular_hardness))

        # Texture maps
        for tex, i in zip(mat.texture_slots.values(), range(len(mat.texture_slots))):
            # Weird thing is that there are always the slots, it's just that
            # if there is no texture in the slot, it's None
            if not tex:
                continue

            if tex.texture_coords != 'UV':
                print("Warning: skipping texture {} because it doesn't use UV texture coordinates!".format(tex.name))
                continue

            # Note that Arkana-FTS only supports png images!
            # I anticipate keeping the correct file path won't do. Some users
            # will pack the texture with absolute path and some with relative.
            #   - Relative, it's all fine, keep the path.
            #   - Absolute, just use the filename and put it in a 'textures' subdir.
            img = tex.texture.image
            imgfile = img.filepath[2:] if img.filepath[:2] == "//" else os.path.join('textures', os.path.basename(img.filepath))
            imgfile_png = os.path.join(os.path.dirname(imgfile), os.path.splitext(os.path.basename(imgfile))[0] + '.png')
            bmat.setprop('uTexture' + (str(i) if i else ''), imgfile_png)

            # And now save the packed (or even just loaded?) image to disk.
            old_filepath = img.filepath
            img.filepath = os.path.join(os.path.dirname(filepath), imgfile_png)
            img.save()
            img.filepath = old_filepath

        bmat.writeXML(file)

    file.close()

def save_matset(filepath, mesh_material_assossiations, materials):
    file = open(filepath, "w", encoding="utf8", newline="\n")
    file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))

    # We have two common use-cases:
    #  1. several sub-meshes (blender meshes/objects) that use different materials,
    #     then we export a default material set with those assossiations.
    #  2. only one mesh with which has several alternative materials
    #     In this case, we create one material set per material.

    if len(mesh_material_assossiations) == 1:
        # use-case 2
        for mat in materials:
            file.write('<MATSET NAME="{}">\n'.format(mat.name))
            for mesh in mesh_material_assossiations.keys():
                file.write('  <LINK SUBMESH="{}">{}</LINK>\n'.format(mesh, mat.name))
            file.write('</MATSET>\n\n')
    else:
        # use-case 1
        file.write('<MATSET NAME="default">\n')
        for mesh, mats in mesh_material_assossiations.items():
            file.write('  <LINK SUBMESH="{}">{}</LINK>\n'.format(mesh, mats[0]))
        file.write('</MATSET>\n\n')

    file.close()

def save_actions(filepath, acts, armatures):
    if(len(acts) == 0):
        return

    file = open(filepath, "w", encoding="utf8", newline="\n")
    file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))

    #fc = bpy.data.actions[0].fcurves[0]
    #bpy.data.objects["Armature"].path_resolve(fc.data_path)[fc.array_index]
    for action in acts:
        banim = common.BougeAnim(action.name)

        for track in action.groups.values():
            # Sometimes, this also contains tracks for bones with no keyframes.
            # Take those out...
            if not track.channels:
                continue

            btrack = common.BougeTrack(track.name)

            # This is the matrix by what the bone is transformed in the armature.
            # (that is, in EDIT MODE.)
            # Note: I found no connection between actions and armatures in blender!
            #       So just look into each one there is.
            boneToWorld = mathutils.Matrix()
            boneToArmature = mathutils.Matrix()
            for armature in armatures:
                if track.name in armature.bones:
                    boneToWorld = armature.bones[track.name].matrix
                    boneToArmature = armature.bones[track.name].matrix_local

            # For now, we sample each track at every single second within the
            # interval.
            # One could argue that that's wasted memory and it's better to really
            # look at the single keyframes. True, but that's much more difficult :)
            # I could mark it as a TODO of low priority..
            for t in range(int(action.frame_range[0]), int(action.frame_range[1]) + 1):

                # TODO: support bone.use_inherit_rotation == False
                def getQuat(group, t):
                    q = None
                    for fcurve in group.channels.values():
                        # TODO We may want to convert other rotation types to quats?
                        if fcurve.data_path[-19:] == "rotation_quaternion":
                            q = mathutils.Quaternion() if not q else q
                            q[fcurve.array_index] = fcurve.evaluate(t)
                    # Bouge stores the rotations relative to the bone itself,
                    # independent of the parents. Blender does the same, thus
                    # we can just use them without any transformation.
                    # Tested in 2.56 and 2.58
                    return q

                def getLoc(group, t):
                    l = None
                    for fcurve in group.channels.values():
                        if fcurve.data_path[-8:] == "location":
                            l = mathutils.Vector() if not l else l
                            l[fcurve.array_index] = fcurve.evaluate(t)
                    # Bouge stores the translations relative to the bone's local
                    # coordinate system (to the bone itself). Blender does the
                    # same, thus we can just use them witout any transformation.
                    # Tested in 2.56 and 2.58
                    return l

                # TODO: support bone.use_inherit_scale == True
                def getScale(group, t):
                    s = None
                    for fcurve in group.channels.values():
                        if fcurve.data_path[-5:] == "scale":
                            s = mathutils.Vector() if not s else s
                            s[fcurve.array_index] = fcurve.evaluate(t)
                    # Bouge (finally) stores the scalings relative to the bone's
                    # local coordinate system (i.e. to the bone itself). Blender
                    # does the same, thus again we can just use the data.
                    # Tested in 2.56 and 2.58
                    return s

                q = getQuat(track, t)
                l = getLoc(track, t)
                s = getScale(track, t)

                if not q and not l and not s:
                    continue

                bkf = common.BougeKeyframe(float(t) / float(bpy.context.scene.render.fps))
                if q:
                    bkf.setRot(q)
                if l:
                    bkf.setTrans(l)
                if s:
                    bkf.setScale(s)
                btrack.addKeyframe(bkf)

            banim.addTrack(btrack)

        banim.writeXML(file)

    file.close()

'''
Currently the exporter lacks these features:
* multiple scene export (only active scene is written)
* particles
'''


def save(operator, context, filepath="",
         use_apply_modifiers=True,       # Yup
         use_rotate_x90=False,           # Halfway (not for skeleton yet)
         use_selection=True,             # Yup
         use_all_scenes=False,           # No
         use_animation=False,            # No
         use_normalize_influences=False, # Yup
         use_filter_in="",               # Fucking shit, to redo...
         use_filter_out="",              # idem..
         only_four_influences=True       # Yup
         ):

    #Window.WaitCursor(1)
    print("exporting bouge: " + filepath)
    time1 = time.clock()

    # Exit edit mode before exporting, so current object states are exported properly.
    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    selected = context.selected_objects
    if use_selection:
        print("using selection")
        objects = selected
    else:
        objects = context.scene.objects

    try:
        # Now we can first deselect all objects. We do this because later on we'll
        # need to enter the armature in order to put it to rest-pose.
        # Hmmm maybe not...
        bpy.ops.object.select_all()
        if len(bpy.context.selected_objects) != 0:
            bpy.ops.object.select_all()

        if use_rotate_x90:
            print("With 90d rotation around X")
            preprocess_matrix = mathutils.Matrix.Rotation(-math.pi / 2.0, 4, 'X')
        else:
            print("Without 90d rotation around X")
            preprocess_matrix = mathutils.Matrix()

        #print(preprocess_matrix)
        #print(type(preprocess_matrix))
        armatures, materials, mesh_material_assossiations = save_mesh(filepath + ".bxmesh", objects, context.scene, preprocess_matrix, use_apply_modifiers, use_normalize_influences, only_four_influences)

        fixedname = filepath + ".bxskel" if len(armatures) <= 1 else None
        for arm in armatures.values():
            arm_preprocess_matrix = preprocess_matrix * arm.matrix_world if use_apply_modifiers else preprocess_matrix
            save_armature(fixedname or filepath + "_" + arm.name + ".bxskel", arm, arm_preprocess_matrix)
        if len(armatures.values()) == 0:
            # If there is no skeleton, we export a dummy skeleton. The lib needs it for now.
            file = open(fixedname, "w", encoding="utf8", newline="\n")
            file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
            file.write('<!-- Automatically exported from Blender v{} using the bouge exporter by Lucas Beyer -->\n'.format(bpy.app.version_string))
            common.BougeSkel('skeleton').writeXML(file)
            file.close()

        save_materials(filepath + ".bxmat", materials.values())
        save_matset(filepath + ".bxmset", mesh_material_assossiations, materials.values())
        save_actions(filepath + ".bxanim", bpy.data.actions.values(), [arm.data for arm in armatures.values()])

    except:
        # Now, re-select the ones that were selected in the beginning:
        for obj in selected:
            obj.select = True
        raise

    # Now, re-select the ones that were selected in the beginning:
    for obj in selected:
        obj.select = True

    print("Bouge Export time: {:.2f}".format(time.clock() - time1))

    #Window.WaitCursor(0)
    return {'FINISHED'}
