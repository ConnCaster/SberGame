#ifndef COMMAND_H
#define COMMAND_H


// @brief Pattern Command
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};


// Класс для управления командами
class CommandManager {
public:
    CommandManager() = default;

    void ExecuteCommand(ICommand* command) {
        command->Execute();
        commands_.push_back(command);

        // Ограничиваем историю команд 3 последними ходами
        if (commands_.size() > 3) {
            commands_.pop_front();
        }
    }

    bool UndoLastCommand() {
        if (commands_.empty()) {
            return false;
        }

        auto& lastCommand = commands_.back();
        lastCommand->Undo();
        commands_.pop_back();
        return true;
    }

private:
    std::deque<ICommand*> commands_;
};

#endif //COMMAND_H
