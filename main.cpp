#include <iostream>
#include <iomanip>
#include "libusb.h"

using namespace std;

void printdev(libusb_device *dev);

std::string printClass(int type);

int main()
{
    libusb_device **devs;           //указатель на указатель на устройство
    libusb_context *ctx = NULL;     //контекст сессии libusb

    int r;          //код ошибок
    ssize_t cnt;    //кол-во устройств

    r = libusb_init(&ctx); //инициализация работы с сессией

    if (r < 0)
    {
        std::cout << "Ошибка: инициализация не выполнена, код:" << r;
        return 1;
    }

    libusb_set_debug(ctx, 3); //задать уровень подробности отладочных сообщений

    cnt = libusb_get_device_list(ctx, &devs);  // получить список всех найденных USB-устройств

    if (cnt < 0)
    {
        std::cout << "Ошибка: список USB устройств не получен." << r;
        return 1;
    }

    std::cout << "найдено устройств: " << cnt << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "* класс устройства" << std::endl;
    std::cout << "|   * идентификатор производителя" << std::endl;
    std::cout << "|   |    * идентификатор устройства" << std::endl;

    std::cout << "+---+----+-----------------------------" << std::endl;

    for (int i = 0; i < cnt; i++) //печатаем данные для каждого USB-устройства из списка
    {
        printdev(devs[i]);
    }

    std::cout << "=======================================\n";

    libusb_free_device_list(devs, 1); //освободить память, выделенную функцией получения списка устройств
    libusb_exit(ctx);   //завершение сессии
    return 0;
}

void printdev(libusb_device *dev)
{
    libusb_device_descriptor desc;          // дескриптор устройства
    libusb_config_descriptor *config;       // дескриптор конфигурации объекта

    const libusb_interface *inter;
    const libusb_interface_descriptor *interdesc;

    int r = libusb_get_device_descriptor(dev, &desc);           //получаем дискриптор устройства

    if (r < 0)
    {
        std::cout << "Ошибка: дескриптор устройства не получен, код:" << r;
        return;
    }

    std::cout << std::setfill('0') << std::setw(2) << (int) desc.bDeviceClass << " ";   //код класса устройства
    std::cout << std::setfill('0') << std::setw(5) << desc.idVendor << " ";             //идентификатор производителя
    std::cout << std::setfill('0') << std::setw(5) << desc.idProduct << " ";            //идентификатор устройства

    libusb_get_config_descriptor(dev, 0, &config);  //получаем конфигурацию устройства
    inter = &config->interface[0];
    interdesc = &inter->altsetting[0];      //инициализация дискриптора интерфейса

    //Высокоурвневая работа скрывает ненужную информацию, отдаляя нас от того, как на самом деле работает компьютер, создаются обстракции.
    //Низкоурвневневая работа позволяет получить полный контроль над происходящим,

    if ((int) desc.bDeviceClass != 0)       //если информация о классе в дискрипторе неизвестена, то смотрим в дискриптор интерфейса
    {
        std::cout << printClass((int) desc.bDeviceClass) << std::endl;
    } else
    {
        std::cout << printClass((int) interdesc->bInterfaceClass) << std::endl;
    }

    unsigned char string[256] = {};        //для серийного номера

    libusb_device_handle *handle = NULL;
    libusb_open(dev, &handle);             //открываем устройство, с которым будем работать

    if (desc.iSerialNumber)
    {
        r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, sizeof(string));
        if (r > 0)
            std::cout << "               " << "Serial number: " << string << std::endl;
    }

    libusb_close(handle);                       //завершаем работу с устройством
    libusb_free_config_descriptor(config);      //освободить память, выделенную функцией получения конфигурации устройства
}


std::string printClass(int type)
{
    switch (type)
    {
        case 0:
            return string("код отсутствует");
            break;
        case 1:
            return string("аудиоустройство");
            break;
        case 2:
            return string("сетевой адаптер");
            break;
        case 3:
            return string("устройство пользовательского интерфейса");
            break;
        case 5:
            return string("физическое устройство");
            break;
        case 6:
            return string("изображения");
            break;
        case 7:
            return string("принтер");
            break;
        case 8:
            return string("устройство хранения данных");
            break;
        case 9:
            return string("концентратор");
            break;
        case 10:
            return string("CDC-Data");
            break;
        case 11:
            return string("Smart Card");
            break;
        case 13:
            return string("Content Security");
            break;
        case 14:
            return string("видеоустройство");
            break;
        case 15:
            return string("персональное медицинское устройство");
            break;
        case 16:
            return string("аудио- и видеоустройства");
            break;
        case 220:
            return string("диагностическое устройство");
            break;
        case 224:
            return string("беспроводный контроллер");
            break;
        case 239:
            return string("различные устройства");
            break;
        case 254:
            return string("специфическое устройство");
            break;
        default:
            return string("Unknown");
            break;

    }
}

