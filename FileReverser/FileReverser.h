#pragma once
#include "IFileReverser.h"
#include "IDataProvider.h"
#include "IEncodingFixer.h"

#include <memory>

class FileReverser : public IFileReverser
{
public:
    FileReverser(std::shared_ptr<IDataProvider> provider, std::shared_ptr<IEncodingFixer> fixer)
        : m_provider(provider), m_fixer(fixer) {}
    virtual ~FileReverser() = default;

    bool process() override;

private:
    std::shared_ptr<IDataProvider> m_provider;
    std::shared_ptr<IEncodingFixer> m_fixer;
};