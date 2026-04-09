#include <windows.h>
#include <iostream>
using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SC_HANDLE hServiceControlManager, hService;

    // 1. Открываем SCM (Диспетчер управления службами)
    // Параметры: NULL (локальный ПК), NULL (активная БД), Права (создание службы)
    hServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (hServiceControlManager == NULL) {
        cout << "Не удалось открыть диспетчер управления службами. Ошибка: " << GetLastError() << endl;
        system("pause");
        return 0;
    }
    cout << "Диспетчер управления службами открыт." << endl;

    // Путь к бинарному файлу (должен существовать!)
    // Внимание: В реальном проекте путь должен быть полным (C:\...\DemoService.exe)
    TCHAR p[] = TEXT("DemoService.exe");
    TCHAR s[] = TEXT("DemoService");   // Внутреннее имя (для SCM)
    TCHAR name[] = TEXT("Demo_Service"); // Отображаемое имя (в services.msc)

    // 2. Создаем службу (добавляем запись в реестр)
    hService = CreateService(
        hServiceControlManager,      // Хэндл SCM
        s,                           // Имя службы
        name,                        // Отображаемое имя
        SERVICE_ALL_ACCESS,          // Полный доступ
        SERVICE_WIN32_OWN_PROCESS,   // Тип: отдельный процесс
        SERVICE_DEMAND_START,        // Тип запуска: Вручную (по требованию)
        SERVICE_ERROR_NORMAL,        // Режим ошибок: Логировать в EventLog
        p,                           // Путь к .exe файлу
        NULL, NULL, NULL, NULL, NULL // Зависимости и учётная запись (LocalSystem по умолчанию)
    );

    if (hService == NULL) {
        cout << "Не удалось создать службу. Ошибка: " << GetLastError() << endl;
        CloseServiceHandle(hServiceControlManager);
        system("pause");
        return 0;
    }

    cout << "Сервис успешно установлен!" << endl;

    // 3. Закрываем хэндлы (освобождаем ресурсы)
    CloseServiceHandle(hService);
    CloseServiceHandle(hServiceControlManager);

    system("pause");
    return 0;
}