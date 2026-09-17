#pragma once

static constexpr uint32_t hash(const char* data, const size_t size) noexcept
{
    uint32_t hash = 5381;

    for (const char* c = data; c < data + size; ++c)
    {
        hash = ((hash << 5) + hash) + (unsigned char)*c;
    }

    return hash;
}

constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
{
    return hash(str, size);
}

static inline RE::BShkbAnimationGraph* GetGraphFromCharacter(RE::hkbCharacter* a_hkbCharacter)
{
    if (!a_hkbCharacter) { return nullptr; }

    return SKSE::stl::adjust_pointer<RE::BShkbAnimationGraph>(a_hkbCharacter, -0xC0);
}