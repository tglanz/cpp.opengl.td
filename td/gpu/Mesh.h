#ifndef __TD_GPU_MESH_H__
#define __TD_GPU_MESH_H__

#include "Geometry.h"
#include "Material.h"
#include "Transformations.h"

namespace td
{
    namespace gpu
    {
        class Mesh
        {
        private:
            Geometry * const m_geometry;
            Material * const m_material;

            Transformations m_transformations;

        public:
            Mesh(Geometry * const geometry, Material * const material) :
                m_geometry(geometry), m_material(material) { }

            inline Geometry * const geometry() { return m_geometry; }
            inline Material * const material() { return m_material; }

            inline Transformations & transformation() { return m_transformations; }

            void draw()
            {
                m_material->assignUniforms();

                m_material->programLayout()->setModelProjectionUniform(m_transformations.getModelMatrix());

                m_geometry->bind();
                m_material->programLayout()->enableAllAttributes();

                m_geometry->drawElements();

                m_material->programLayout()->disableAllAttributes();
                m_geometry->unbind();
            }
        };
    }
}

#endif // __TD_GPU_MESH_H__
