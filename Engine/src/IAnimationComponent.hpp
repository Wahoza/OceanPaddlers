//Author: Radomir Dinic
//Description: Would-be animation component, currently does nothing
//License: LICENSE.txt
#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{
class IAnimationComponent : public IComponent
{
public:
    using Ptr = std::shared_ptr<IAnimationComponent>;

    using IComponent::IComponent;
};
} // namespace mmt_gd
