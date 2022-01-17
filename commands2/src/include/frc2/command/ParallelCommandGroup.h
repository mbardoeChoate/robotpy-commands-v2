// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4521)
#endif

#include <memory>
#include <utility>
#include <vector>

#include "frc2/command/CommandGroupBase.h"
#include "frc2/command/CommandHelper.h"

namespace frc2 {
/**
 * A CommandGroup that runs a set of commands in parallel, ending when the last
 * command ends.
 *
 * <p>As a rule, CommandGroups require the union of the requirements of their
 * component commands.
 *
 * This class is provided by the NewCommands VendorDep
 */
class ParallelCommandGroup
    : public CommandGroupBase {
 public:
  /**
   * Creates a new ParallelCommandGroup.  The given commands will be executed
   * simultaneously. The command group will finish when the last command
   * finishes.  If the CommandGroup is interrupted, only the commands that are
   * still running will be interrupted.
   *
   * @param commands the commands to include in this group.
   */
  explicit ParallelCommandGroup(
      std::vector<std::shared_ptr<Command>>&& commands);

  /**
   * Creates a new ParallelCommandGroup.  The given commands will be executed
   * simultaneously. The command group will finish when the last command
   * finishes.  If the CommandGroup is interrupted, only the commands that are
   * still running will be interrupted.
   *
   * @param commands the commands to include in this group.
   */
  template <class... Types,
            typename = std::enable_if_t<std::conjunction_v<
                std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  explicit ParallelCommandGroup(Types&&... commands) {
    AddCommands(std::forward<Types>(commands)...);
  }

  ParallelCommandGroup(ParallelCommandGroup&& other) = default;

  // No copy constructors for commandgroups
  ParallelCommandGroup(const ParallelCommandGroup&) = delete;

  // Prevent template expansion from emulating copy ctor
  ParallelCommandGroup(ParallelCommandGroup&) = delete;

  template <class... Types,
            typename = std::enable_if_t<std::conjunction_v<
                std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  void AddCommands(Types&&... commands) {
    std::vector<std::shared_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_shared<std::remove_reference_t<Types>>(
         std::forward<Types>(commands))),
     ...);
    AddCommands(std::move(foo));
  }

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

  bool RunsWhenDisabled() const override;

 public:
  void AddCommands(std::vector<std::shared_ptr<Command>>&& commands) final;
 private:
  std::vector<std::pair<std::shared_ptr<Command>, bool>> m_commands;
  bool m_runWhenDisabled{true};
  bool isRunning = false;
};
}  // namespace frc2

#ifdef _WIN32
#pragma warning(pop)
#endif
