#pragma once

#include <system_error>
#include <filesystem>
#include <string>
#include <optional>
#include <queue>


// Перечисление языков программирования
enum ProgramLanguages:uint16_t
{
	verilog = 0,
	cpp,
	python
};

// Структура исходного файла
struct CodeFileData 
{
    std::string filename;           // Имя файла
    std::vector<std::string> lines; // Содержимое файла построчно
};

// Структура запроса на выполнение кода
struct CodeExecRequest 
{
    bool has_timeout = true;            // Флаг наличия ограничения выполнения по времени
    uint16_t timeout_ms = 5000;         // Максимальное время выполнения в милисекундах (по умолчанию 5000)
    ProgramLanguages language;          // Язык программирования
    std::string predef_console_input;   // Заранее заданный ввод консоли
    std::string compile_flags;          // Флаги компиляции
    std::string run_args;               // Параметры запуска
    std::vector<CodeFileData> files;    // Список файлов с кодом (главным ожидается элемент 0)
};

// Структура для передачи в выполняемую программу данных
struct ConsoleInput
{
    std::string input; // Ввод консоли
};

// Структура ответа среды при выполнении кода
struct CodeExecAnswer
{
    // Перечисление состояний программы
    enum StatusType :uint8_t { has_ended, waits_input, stage_result };

    StatusType status;  // Состояние программы
    std::string reply;  // Вывод среды исполнения программы
};

// Менеджер исполнения программ
class CodeExecManager
{
private:
    // Очередь сообщений
    std::queue<CodeExecAnswer> answer_arr;
    // тут ещё должен быть флаг или другой способ сигнализации о наличии сообщений
    
public:
    // Функция для запуска кода
    void RunCode(const CodeExecRequest& request);

    // Ввод в программу в процессе выполнения
    void SendConsoleToProg(const ConsoleInput& input);

    // Получить последнее сообщение из очереди
    CodeExecAnswer GetLastMessage();

private:
    // Функция для выдачи сообщений программой
    void PutAnswerToQueue();

    /*
    Логика такая:
    1. Класс создаётся и существует в отдельном потоке
       Выполнение в другом потоке необходимо для предотвращения зависания или падения основной программы
    2. В какой-то момент в нём запускается код через RunCode
    3. По мере выполнения кода, класс создаёт ответы и помещает их в очередь, попутно сигнализируя о наличии ответа
    4. Основная программа, при поступлении сигнала, забирает готовый ответ функцией GetLastMessage, что убирает ответ из очереди
    5. В конце выполнения идёт последний ответ с типом has_ended
    
    */
};