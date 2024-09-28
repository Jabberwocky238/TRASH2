#pragma once
#include "core.h"

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <memory>

struct ZFdNode: public std::enable_shared_from_this<ZFdNode>
{
    using ZIdentifier = std::string;
    using ZParent = std::optional<std::weak_ptr<ZFdNode>>;

    ZIdentifier id;
    uintmax_t size;
    time_t last_modified;
    int children_count;

    ZParent parent;
    std::vector<std::shared_ptr<ZFdNode>> children;

    ZFdNode(const ZIdentifier &id, ZParent parent);
    ZFdNode(const ZIdentifier &id);
    ~ZFdNode();

    std::string print();
    std::filesystem::path path();
    void scan();
    bool verify();
    // locate a node by its path
    std::shared_ptr<ZFdNode> locate(const ZIdentifier &id);
};
