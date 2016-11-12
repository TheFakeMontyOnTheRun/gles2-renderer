//
// Created by Daniel Monteiro on 12/11/16.
//
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VBORenderingJob.h"

glm::mat4 odb::VBORenderingJob::getTransform() {
    return mVBOInstanceTransform;
}

int odb::VBORenderingJob::getVBOId() {
    return mVBOId;
}

odb::VBORenderingJob::VBORenderingJob(int aVBOId, int aVBOIndicesId, int aAmount, glm::mat4 aTransform, float aShade) : mVBOId(aVBOId), mVBOIndicesId(aVBOIndicesId), mAmount(aAmount), mVBOInstanceTransform( aTransform), mShade(aShade) {
}

float odb::VBORenderingJob::getShade() {
    return mShade;
}

int odb::VBORenderingJob::getAmount() {
    return mAmount;
}

int odb::VBORenderingJob::getVBOIndicesId() {
    return mVBOIndicesId;
}











