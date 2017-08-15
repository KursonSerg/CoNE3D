#ifndef MATERIAL_H
#define MATERIAL_H

#include <array>

#include <CTexture.h>
#include <Shaders/Program.h> // TODO: Remove this dependency

class CMaterial final
{
public:
    void setDiffuseTexture(CTexture *texture);
    void setNormalTexture(CTexture *texture);

    void bind() const;

private:
    std::array<std::unique_ptr<CTexture>, MAX_TEXTURE_UNITS> _textures;
};

#endif // MATERIAL_H
