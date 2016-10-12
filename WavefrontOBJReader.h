//
// Created by monty on 24/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_WAVEFRONTOBJREADER_H
#define DUNGEONSOFNOUDAR_NDK_WAVEFRONTOBJREADER_H

std::shared_ptr<odb::Scene> readScene(std::istream& objFileContents, std::istream& materialFileContents);

#endif //DUNGEONSOFNOUDAR_NDK_WAVEFRONTOBJREADER_H
