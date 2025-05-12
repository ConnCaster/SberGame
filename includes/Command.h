#ifndef COMMAND_H
#define COMMAND_H


// Базовый класс команды
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
        commands_.push(command);

        // Ограничиваем историю команд 3 последними ходами
        if (commands_.size() > 3) {
            commands_.pop();
        }
    }

    bool UndoLastCommand() {
        if (commands_.empty()) {
            return false;
        }

        auto& lastCommand = commands_.top();
        lastCommand->Undo();
        commands_.pop();
        return true;
    }

private:
    std::stack<ICommand*> commands_;
};

#endif //COMMAND_H
