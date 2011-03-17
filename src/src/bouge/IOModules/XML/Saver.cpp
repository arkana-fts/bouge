////////////////////////////////////////////////////////////
//
// Bouge - Modern and flexible skeletal animation library
// Copyright (C) 2010 Lucas Beyer (pompei2@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include <bouge/IOModules/XML/Saver.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLSerializer.hpp>

#include <bouge/CoreMesh.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreMaterial.hpp>
#include <bouge/CoreMaterialSet.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/CoreTrack.hpp>
#include <bouge/CoreKeyframe.hpp>
#include <bouge/Math/Util.hpp>
#include <bouge/Math/TimeFunction.hpp>
#include <bouge/Util.hpp>

#include <sstream>

namespace bouge
{

    XMLSaver::XMLSaver()
    { }

    XMLSaver::~XMLSaver()
    { }

    std::vector<char> XMLSaver::saveMesh(CoreMeshPtrC mesh)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        out.openTag("MESH")
            .attribute("NAME", mesh->name());

        for(CoreMesh::const_iterator iSubMesh = mesh->begin() ; iSubMesh != mesh->end() ; ++iSubMesh) {
            out.openTag("SUBMESH")
                .attribute("NAME", iSubMesh->name());

            for(std::size_t iVtx = 0 ; iVtx < iSubMesh->vertexCount() ; ++iVtx) {
                Vertex vtx = iSubMesh->vertex(iVtx);

                out.openTag("VERTEX");

                std::stringstream ss;
                ss << vtx.pos().x() << " " << vtx.pos().y() << " " << vtx.pos().z();
                out.openTag("POS").text(ss.str()).closeTag();

                for(Vertex::iterator i = vtx.begin() ; i != vtx.end() ; ++i) {
                    out.openTag("ATTRIB").attribute("TYPE", i.name()).text(to_s(i.value())).closeTag();
                }

                for(std::size_t iInfluence = 0 ; iInfluence < vtx.influenceCount() ; ++iInfluence) {
                    out.openTag("INFLUENCE")
                        .attribute("BONE", vtx.influence(iInfluence).sBoneName)
                        .text(to_s(vtx.influence(iInfluence).w))
                    .closeTag();
                }

                out.closeTag(); // VERTEX
            }

            for(std::size_t iFace = 0 ; iFace < iSubMesh->faceCount() ; ++iFace) {
                Face face = iSubMesh->face(iFace);

                out.openTag("FACE");

                for(std::vector<Face::index_t>::const_iterator iIdx = face.idxs().begin() ; iIdx != face.idxs().end() ; ++iIdx) {
                    out.openTag("VERTEXID").text(to_s(*iIdx)).closeTag();
                }

                out.closeTag(); // FACE
            }

            out.closeTag(); // SUBMESH
        }

        out.closeTag(); // MESH

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveMesh(CoreMeshPtrC mesh, const std::string& sFileName)
    {
        Saver::saveMesh(mesh, sFileName);
    }

    // Small helper function. This is implementation detail, that's why it
    // neither is in the header nor is it in the class.
    XMLSerializer<std::stringstream>& storeBoneRecursive(XMLSerializer<std::stringstream>& out, CoreBonePtrC bone)
    {
        out.openTag("BONE")
            .attribute("NAME", bone->name())
            .attribute("LENGTH", to_s(bone->length()))
            .attribute("RELPOSITION", to_s(bone->relativeRootPosition()))
            .attribute("RELROTATION", to_s(bone->relativeBoneRotation()));

        for(CoreBone::const_iterator iChildBone = bone->begin() ; iChildBone != bone->end() ; ++iChildBone) {
            storeBoneRecursive(out, *iChildBone);
        }

        return out.closeTag();
    }

    std::vector<char> XMLSaver::saveSkeleton(CoreSkeletonPtrC skel)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        out.openTag("SKELETON")
            .attribute("NAME", skel->name());
        for(CoreSkeleton::const_root_iterator iRootBone = skel->begin_root() ; iRootBone != skel->end_root() ; ++iRootBone) {
            storeBoneRecursive(out, *iRootBone);
        }
        out.closeTag(); // SKELETON

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveSkeleton(CoreSkeletonPtrC skel, const std::string& sFileName)
    {
        Saver::saveSkeleton(skel, sFileName);
    }

    void saveMatImpl(XMLSerializer<std::stringstream>& out, CoreMaterialPtrC mat)
    {
        out.openTag("MATERIAL")
            .attribute("NAME", mat->name());
        for(CoreMaterial::const_iterator iProperty = mat->begin() ; iProperty != mat->end() ; ++iProperty) {
            out.openTag("PROPERTY")
                .attribute("NAME", iProperty.name())
                .text(iProperty.value())
                .closeTag();
        }
        out.closeTag(); // MATERIAL
    }

    std::vector<char> XMLSaver::saveMaterial(CoreMaterialPtrC mat)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        saveMatImpl(out, mat);

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveMaterial(CoreMaterialPtrC mat, const std::string& sFileName)
    {
        Saver::saveMaterial(mat, sFileName);
    }

    std::vector<char> XMLSaver::saveMaterials(const std::vector<CoreMaterialPtrC>& mats)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreMaterialPtrC>::const_iterator iMat = mats.begin() ; iMat != mats.end() ; ++iMat) {
            saveMatImpl(out, *iMat);
        }

        return Saver::toData(ssOut);
    }

    std::vector<char> XMLSaver::saveMaterials(const std::vector<CoreMaterialPtr>& mats)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreMaterialPtr>::const_iterator iMat = mats.begin() ; iMat != mats.end() ; ++iMat) {
            saveMatImpl(out, *iMat);
        }

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveMaterials(const std::vector<CoreMaterialPtrC>& mats, const std::string& sFileName)
    {
        Saver::saveMaterials(mats, sFileName);
    }

    void XMLSaver::saveMaterials(const std::vector<CoreMaterialPtr>& mats, const std::string& sFileName)
    {
        Saver::saveMaterials(mats, sFileName);
    }

    void saveMatSetImpl(XMLSerializer<std::stringstream>& out, CoreMaterialSetPtrC matset)
    {
        out.openTag("MATSET")
            .attribute("NAME", matset->name());
        for(CoreMaterialSet::const_iterator iAssos = matset->begin() ; iAssos != matset->end() ; ++iAssos) {
            out.openTag("LINK")
                .attribute("SUBMESH", iAssos.meshname())
                .text(iAssos.matname())
                .closeTag();
        }
        out.closeTag(); // MATSET
    }

    std::vector<char> XMLSaver::saveMaterialSet(CoreMaterialSetPtrC matset)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        saveMatSetImpl(out, matset);

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveMaterialSet(CoreMaterialSetPtrC matset, const std::string& sFileName)
    {
        Saver::saveMaterialSet(matset, sFileName);
    }

    std::vector<char> XMLSaver::saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreMaterialSetPtrC>::const_iterator iMatset = matsets.begin() ; iMatset != matsets.end() ; ++iMatset) {
            saveMatSetImpl(out, *iMatset);
        }

        return Saver::toData(ssOut);
    }

    std::vector<char> XMLSaver::saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreMaterialSetPtr>::const_iterator iMatset = matsets.begin() ; iMatset != matsets.end() ; ++iMatset) {
            saveMatSetImpl(out, *iMatset);
        }

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& mats, const std::string& sFileName)
    {
        Saver::saveMaterialSets(mats, sFileName);
    }

    void XMLSaver::saveMaterialSets(const std::vector<CoreMaterialSetPtr>& mats, const std::string& sFileName)
    {
        Saver::saveMaterialSets(mats, sFileName);
    }

    void saveAnimImpl(XMLSerializer<std::stringstream>& out, CoreAnimationPtrC anim)
    {
        out.openTag("ANIMATION")
            .attribute("NAME", anim->name());

        // This is a rough estimate of the wanted control function.
        if(dynamic_cast<const LinearTF*>(anim->preferredControl())) {
            out.attribute("ENDCONTROL", "continue");
        } else if(dynamic_cast<const RepeatTF*>(anim->preferredControl())) {
            out.attribute("ENDCONTROL", "repeat");
        } else if(dynamic_cast<const CycleTF*>(anim->preferredControl())) {
            out.attribute("ENDCONTROL", "cycle");
        } else if(dynamic_cast<const HoldTF*>(anim->preferredControl())) {
            const HoldTF* htf = dynamic_cast<const HoldTF*>(anim->preferredControl());
            if(nearZero(htf->valueFrom())) {
                out.attribute("ENDCONTROL", "reset");
            } else {
                out.attribute("ENDCONTROL", "hold");
            }
        } else {
            // We default to the current bouge's default instead of adding
            // nothing, to secure against future default changes.
            out.attribute("ENDCONTROL", "repeat");
        }

        for(CoreAnimation::const_iterator iTrack = anim->begin() ; iTrack != anim->end() ; ++iTrack) {
            out.openTag("TRACK")
                .attribute("BONE", iTrack.bone());

            for(CoreTrack::const_iterator iKeyframe = iTrack->begin() ; iKeyframe != iTrack->end() ; ++iKeyframe) {
                out.openTag("KEYFRAME")
                    .attribute("TIME", to_s(iKeyframe.time()));

                    if(iKeyframe->hasTranslation()) {
                        out.openTag("TRANSLATION")
                            .text(to_s(iKeyframe->translation()))
                            .closeTag();
                    }

                    if(iKeyframe->hasRotation()) {
                        out.openTag("ROTATION")
                            .text(to_s(iKeyframe->rotation()))
                            .closeTag();
                    }

                    if(iKeyframe->hasScale()) {
                        out.openTag("SCALE")
                            .text(to_s(iKeyframe->scale()))
                            .closeTag();
                    }

                out.closeTag(); // KEYFRAME
            }

            out.closeTag(); // TRACK
        }
        out.closeTag(); // ANIMATION
    }

    std::vector<char> XMLSaver::saveAnimation(CoreAnimationPtrC anim)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        saveAnimImpl(out, anim);

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveAnimation(CoreAnimationPtrC anim, const std::string& sFileName)
    {
        Saver::saveAnimation(anim, sFileName);
    }

    std::vector<char> XMLSaver::saveAnimations(const std::vector<CoreAnimationPtrC>& anims)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreAnimationPtrC>::const_iterator iAnim = anims.begin() ; iAnim != anims.end() ; ++iAnim) {
            saveAnimImpl(out, *iAnim);
        }

        return Saver::toData(ssOut);
    }

    std::vector<char> XMLSaver::saveAnimations(const std::vector<CoreAnimationPtr>& anims)
    {
        std::stringstream ssOut;
        XMLSerializer<std::stringstream> out(ssOut);

        for(std::vector<CoreAnimationPtr>::const_iterator iAnim = anims.begin() ; iAnim != anims.end() ; ++iAnim) {
            saveAnimImpl(out, *iAnim);
        }

        return Saver::toData(ssOut);
    }

    void XMLSaver::saveAnimations(const std::vector<CoreAnimationPtrC>& anims, const std::string& sFileName)
    {
        Saver::saveAnimations(anims, sFileName);
    }

    void XMLSaver::saveAnimations(const std::vector<CoreAnimationPtr>& anims, const std::string& sFileName)
    {
        Saver::saveAnimations(anims, sFileName);
    }

}
