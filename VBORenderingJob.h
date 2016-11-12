//
// Created by Daniel Monteiro on 12/11/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_VBORENDERINGJOB_H
#define DUNGEONSOFNOUDAR_NDK_VBORENDERINGJOB_H

namespace odb {
    class VBORenderingJob {
    public:
        explicit VBORenderingJob( int vboId, int vboIndicesId, int amount, glm::mat4 transform, float shade );
        glm::mat4 getTransform();
        int getVBOId();
        int getVBOIndicesId();
        float getShade();
        int getAmount();
    private:
        glm::mat4 mVBOInstanceTransform = glm::mat4( 1.0f );
        float mShade = 1.0f;
        int mVBOId = -1;
        int mVBOIndicesId = -1;
        int mAmount = 0;
    };
}

#endif //DUNGEONSOFNOUDAR_NDK_VBORENDERINGJOB_H