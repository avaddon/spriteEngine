//
//  seSprite.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seSprite_hpp
#define seSprite_hpp

#include "seSceneObject.hpp"

namespace spriteEngine {
    class seTexture;

    class seSprite : public seSceneObject {
    public:
        seSprite(bool centred, seProgram *shaderProgram, seTexture *texture);
        virtual ~seSprite();

        void SetTexture(seTexture *texture);

        virtual seTexture* GetTexture() { return m_texture; }

        virtual void Render();

    protected:
        virtual void Bind();
        virtual void Unbind();

        seTexture *m_texture;
    };
}

#endif /* seSprite_hpp */
