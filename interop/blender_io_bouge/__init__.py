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

bl_info = {
    "name": "Bouge model format",
    "author": "Lucas Beyer",
    "location": "File > Import-Export",
    "description": "Import-Export bouge XML and binary files",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    #"support": 'OFFICIAL',
    "wiki_url" : "http://arkana-fts.org/wiki",
    "tracker_url" : "http://arkana-fts.org/board",
    "category": "Import-Export"
}

# To support reload properly, try to access a package var, if it's there, reload everything
if "bpy" in locals():
    import imp
#    if "import_bouge" in locals():
#        imp.reload(import_bouge)
    if "export_bouge" in locals():
        imp.reload(export_bouge)


import bpy
from bpy.props import *
try:
    # 2.58
    from bpy_extras.io_utils import ExportHelper, ImportHelper
except ImportError:
    # 2.56
    from io_utils import ExportHelper, ImportHelper

#class ImportBouge(bpy.types.Operator, ImportHelper):
#    pass
    #'''Load a Wavefront OBJ File'''
    #bl_idname = "import_scene.obj"
    #bl_label = "Import OBJ"

    #filename_ext = ".obj"
    #filter_glob = StringProperty(default="*.obj;*.mtl", options={'HIDDEN'})

    #CREATE_SMOOTH_GROUPS = BoolProperty(name="Smooth Groups", description="Surround smooth groups by sharp edges", default=True)
    #CREATE_FGONS = BoolProperty(name="NGons as FGons", description="Import faces with more then 4 verts as fgons", default=True)
    #CREATE_EDGES = BoolProperty(name="Lines as Edges", description="Import lines and faces with 2 verts as edge", default=True)
    #SPLIT_OBJECTS = BoolProperty(name="Object", description="Import OBJ Objects into Blender Objects", default=True)
    #SPLIT_GROUPS = BoolProperty(name="Group", description="Import OBJ Groups into Blender Objects", default=True)
    ## old comment: only used for user feedback
    ## disabled this option because in old code a handler for it disabled SPLIT* params, it's not passed to load_obj
    ## KEEP_VERT_ORDER = BoolProperty(name="Keep Vert Order", description="Keep vert and face order, disables split options, enable for morph targets", default= True)
    #ROTATE_X90 = BoolProperty(name="-X90", description="Rotate X 90.", default=True)
    #CLAMP_SIZE = FloatProperty(name="Clamp Scale", description="Clamp the size to this maximum (Zero to Disable)", min=0.0, max=1000.0, soft_min=0.0, soft_max=1000.0, default=0.0)
    #POLYGROUPS = BoolProperty(name="Poly Groups", description="Import OBJ groups as vertex groups.", default=True)
    #IMAGE_SEARCH = BoolProperty(name="Image Search", description="Search subdirs for any assosiated images (Warning, may be slow)", default=True)

    #def execute(self, context):
        ## print("Selected: " + context.active_object.name)
        #from . import import_obj
        #return import_obj.load(self, context, **self.as_keywords(ignore=("filter_glob",)))


class ExportBouge(bpy.types.Operator, ExportHelper):
    '''Save a Bouge model File'''

    bl_idname = "export_scene.bouge"
    bl_label = 'Export Bouge'
    bl_options = {'PRESET'}

    filename_ext = ""
    filter_glob = StringProperty(default="*.bx*", options={'HIDDEN'})

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.

    # context group
    use_selection = BoolProperty(name="Selection Only", description="Export selected objects only", default=False)
    #use_all_scenes = BoolProperty(name="All Scenes", description="", default=False)
    #use_animation = BoolProperty(name="Animation", description="", default=False)

    # object group
    use_apply_modifiers = BoolProperty(name="Apply Modifiers (/!\)", description="Apply modifiers (current frame has to be in rest pose!)", default=False)
    #use_rotate_x90 = BoolProperty(name="Rotate X90", description="Doesn't currently work in all cases.", default=False)

    use_normalize_influences = BoolProperty(name="Normalize influences", description="Normalize influences (bone weights)", default=True)
    only_four_influences = BoolProperty(name="Restrict to 4 influences", description="Drops weakest influences if a vertex got more than 4 influences.", default=True)

    # extra data group
    #use_edges = BoolProperty(name="Edges", description="", default=True)
    #use_normals = BoolProperty(name="Normals", description="", default=False)
    #use_hq_normals = BoolProperty(name="High Quality Normals", description="", default=True)
    #use_uvs = BoolProperty(name="UVs", description="", default=True)
    #use_materials = BoolProperty(name="Materials", description="", default=True)
    #copy_images = BoolProperty(name="Copy Images", description="", default=False)
    #use_triangles = BoolProperty(name="Triangulate", description="", default=False)
    #use_vertex_groups = BoolProperty(name="Polygroups", description="", default=False)
    #use_nurbs = BoolProperty(name="Nurbs", description="", default=False)

    # grouping group
    #use_blen_objects = BoolProperty(name="Objects as OBJ Objects", description="", default=True)
    #group_by_object = BoolProperty(name="Objects as OBJ Groups ", description="", default=False)
    #group_by_material = BoolProperty(name="Material Groups", description="", default=False)
    #keep_vertex_order = BoolProperty(name="Keep Vertex Order", description="", default=False)

    def execute(self, context):
        from . import export_bouge
        return export_bouge.save(self, context, **self.as_keywords(ignore=("check_existing", "filter_glob")))


#def menu_func_import(self, context):
#    self.layout.operator(ImportBouge.bl_idname, text="Bouge (.bx*)")


def menu_func_export(self, context):
    self.layout.operator(ExportBouge.bl_idname, text="Bouge (.bx*)")


def register():
    bpy.utils.register_module(__name__)

#    bpy.types.INFO_MT_file_import.append(menu_func_import)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

#    bpy.types.INFO_MT_file_import.remove(menu_func_import)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


# CONVERSION ISSUES
# - matrix problem
# - duplis - only tested dupliverts
# - all scenes export
# + normals calculation

if __name__ == "__main__":
    register()
