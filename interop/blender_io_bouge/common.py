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

import bpy
import mathutils

class BougeVertex:
    def __init__(self, co, no, uvs, influences):
        self.co = co
        self.no = no
        self.uv = uvs
        self.influences = influences

    def writeXML(self, file):
        file.write('  <VERTEX><POS>{:.5f} {:.5f} {:.5f}</POS>\n'.format(*self.co))
        file.write('          <ATTRIB TYPE="normal">{:.5f} {:.5f} {:.5f}</ATTRIB>\n'.format(*self.no))
        for (uv, i) in zip(self.uv, range(len(self.uv))):
            file.write('          <ATTRIB TYPE="texcoord{}">{:.5f} {:.5f}</ATTRIB>\n'.format(i, *uv))
        for influence in self.influences:
            file.write('          <INFLUENCE BONE="{}">{:.5f}</INFLUENCE>\n'.format(*influence))
        file.write('  </VERTEX>\n')

    def __hash__(self):
        #return hash(tuple(sum([[c, n, u] for c in self.co for n in self.no for u in self.uv], [])))
        return hash(str(self))

    def __eq__(self, other):
        return str(self) == str(other)

    def __str__(self):
        first = "co: {:.5f} {:.5f} {:.5f}, no: {:.5f} {:.5f} {:.5f}".format(self.co[0], self.co[1], self.co[2], self.no[0], self.no[1], self.no[2])
        second = ', '.join(['uv: {:.5f}, {:.5f}'.format(*uv) for uv in self.uv])
        third = ', '.join(['inf from {}: {:.5f}'.format(*influence) for influence in self.influences])
        return ', '.join([first, second, third])

class BougeSubmesh:
    def __init__(self, name):
        self.name = name
        self.verts = list()
        self.fidxs = list()
        self.vertsLookup = dict()

    def __addBougeVert(self, v):
        if v not in self.vertsLookup:
            self.verts.append(v)
            print("added vert {}".format(v))
            return self.vertsLookup.setdefault(v, len(self.verts)-1)
        else:
            return self.vertsLookup[v]

    def __makeBougeVert(self, use_normalize_influences, obj, me, n, f, fvert):
        vtx = me.vertices[f.vertices[fvert]]
        co = vtx.co
        no = vtx.normal if n == None else n

        #That is: influences = [(BoneName1, w1), (BoneName2, w2), ..., nInfluences]
        influences = [(obj.vertex_groups.values()[influence.group].name, influence.weight) for influence in vtx.groups.values()]
        if use_normalize_influences:
            total_w = sum(w for bname,w in influences)
            influences = [(bname,w/total_w) for bname,w in influences]

        #That is: uvs = [(u1, v1), (u2, v2), ..., nMaps]
        uvs = [uvmap.data.values()[f.index].uv[fvert] for uvmap in me.uv_textures.values()]
        uvs = [(uv[0], 1.0 - uv[1]) for uv in uvs] # The usual trick needed...

        return BougeVertex(co, no, uvs, influences)

    def addFace(self, use_normalize_influences, obj, me, n, f, fvert1, fvert2, fvert3):
        bv1 = self.__makeBougeVert(use_normalize_influences, obj, me, n, f, fvert1)
        bv2 = self.__makeBougeVert(use_normalize_influences, obj, me, n, f, fvert2)
        bv3 = self.__makeBougeVert(use_normalize_influences, obj, me, n, f, fvert3)
        self.fidxs.append((self.__addBougeVert(bv1), self.__addBougeVert(bv2), self.__addBougeVert(bv3)))

    def writeXML(self, file):
        file.write('  <SUBMESH NAME="{}">\n'.format(self.name))
        for v in self.verts:
            v.writeXML(file)
        for f in self.fidxs:
            file.write('    <FACE><VERTEXID>{}</VERTEXID><VERTEXID>{}</VERTEXID><VERTEXID>{}</VERTEXID></FACE>\n'.format(*f))
        file.write('  </SUBMESH>\n')

class BougeMesh:
    def __init__(self, name):
        self.name = name
        self.submeshes = []

    def newsubmesh(self, name):
        self.submeshes.append(BougeSubmesh(name))
        return self.submeshes[-1]

    def writeXML(self, file):
        file.write('<MESH NAME="{}">\n'.format(self.name))
        for submesh in self.submeshes:
            submesh.writeXML(file)
        file.write('</MESH>\n\n')

class BougeBone:
    def __init__(self, name, len, relPos, relRot):
        self.name = name
        self.len = len
        self.relPos = relPos
        self.relRot = relRot
        self.children = list()

    def giveChild(self, child):
        self.children.append(child)

    def writeXML(self, file, spaces = 2):
        file.write(' ' * spaces + '<BONE NAME="{}" LENGTH="{:.5f}" RELPOSITION="{:.5f} {:.5f} {:.5f}" RELROTATION="{:.5f} {:.5f} {:.5f} {:.5f}">\n'.format(self.name, self.len, self.relPos.x, self.relPos.y, self.relPos.z, self.relRot.x, self.relRot.y, self.relRot.z, self.relRot.w))
        for child in self.children:
            child.writeXML(file, spaces + 2)
        file.write(' ' * spaces + '</BONE>\n')

class BougeSkel:
    def __init__(self, name):
        self.name = name
        self.rootbones = list()

    def giveRootBone(self, bone):
        self.rootbones.append(bone)

    def writeXML(self, file):
        file.write('<SKELETON NAME="{}">\n'.format(self.name))
        for rootbone in self.rootbones:
            rootbone.writeXML(file)
        file.write('</SKELETON>\n\n')

class BougeMaterial:
    def __init__(self, name):
        self.name = name
        self.props = dict()

    def setprop(self, name, val):
        self.props[name] = val

    def writeXML(self, file):
        file.write('<MATERIAL NAME="{}">\n'.format(self.name))
        for name,val in self.props.items():
            file.write('  <PROPERTY NAME="{}">{}</PROPERTY>\n'.format(name, val))
        file.write('</MATERIAL>\n\n')

class BougeKeyframe:
    def __init__(self, time):
        self.time = time
        self.trans = None
        self.rot = None
        self.scale = None

    def setTrans(self, trans):
        self.trans = trans

    def setRot(self, rot):
        self.rot = rot

    def setScale(self, scale):
        self.scale = scale

    def writeXML(self, file):
        file.write('    <KEYFRAME TIME="{:.5f}">\n'.format(self.time))
        if self.trans:
            file.write('      <TRANSLATION>{:.5f} {:.5f} {:.5f}</TRANSLATION>\n'.format(*self.trans))
        if self.rot:
            # Note that blender quaternions are stored as w, x, y, z while bouge quaternions are x, y, z, w
            file.write('      <ROTATION>{:.5f} {:.5f} {:.5f} {:.5f}</ROTATION>\n'.format(self.rot.x, self.rot.y, self.rot.z, self.rot.w))
        if self.scale:
            file.write('      <SCALE>{:.5f} {:.5f} {:.5f}</SCALE>\n'.format(*self.scale))
        file.write('    </KEYFRAME>\n')

class BougeTrack:
    def __init__(self, name):
        self.name = name
        self.keyframes = list()

    def addKeyframe(self, kf):
        self.keyframes.append(kf)

    def writeXML(self, file):
        file.write('  <TRACK BONE="{}">\n'.format(self.name))
        for kf in self.keyframes:
            kf.writeXML(file)
        file.write('  </TRACK>\n')
        pass

class BougeAnim:
    def __init__(self, name):
        self.name = name
        self.tracks = list()

    def addTrack(self, track):
        self.tracks.append(track)

    def writeXML(self, file):
        file.write('<ANIMATION NAME="{}" ENDCONTROL="{}">\n'.format(self.name, "repeat"))
        for track in self.tracks:
            track.writeXML(file)
        file.write('</ANIMATION>\n\n')
