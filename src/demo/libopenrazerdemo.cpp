#include "libopenrazer.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

// Main method for testing / playing.
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({ "backend", "libopenrazer backend to use (openrazer/razer_test)", "backend" });
    parser.process(app);

    QString chosenBackend = parser.value("backend");

    libopenrazer::Manager *manager;
    if (chosenBackend == "" || chosenBackend == "openrazer") {
        manager = new libopenrazer::openrazer::Manager();
    } else if (chosenBackend == "razer_test") {
        manager = new libopenrazer::razer_test::Manager();
    } else {
        parser.showHelp();
    }

    qDebug() << "Daemon running:" << manager->isDaemonRunning();
    qDebug() << "Daemon version:" << manager->getDaemonVersion();
    qDebug() << "Supported devices:" << manager->getSupportedDevices();
    manager->syncEffects(false);

    foreach (const QDBusObjectPath &devicePath, manager->getDevices()) {
        qDebug() << "-----------------";
        libopenrazer::Device *device = manager->getDevice(devicePath);
        qDebug() << "Devicename:" << device->getDeviceName();
        qDebug() << "Firmware version:" << device->getFirmwareVersion();
        try { // fake driver doesn't have device_mode
            qDebug() << "Devicemode:" << device->getDeviceMode();
        } catch (QException &e) {
        }
        // device->setDeviceMode(0x03, 0x00);
        // qDebug() << "Devicemode:" << device->getDeviceMode();
        qDebug() << "Serial: " << device->getSerial();

        if (device->hasFeature("dpi")) {
            qDebug() << "DPI";
            openrazer::RazerDPI dpi = device->getDPI();
            qDebug() << dpi.dpi_x << dpi.dpi_y;
            device->setDPI({ 500, 500 });
            dpi = device->getDPI();
            qDebug() << dpi.dpi_x << dpi.dpi_y;
            qDebug() << "maxdpi: " << device->maxDPI();
        }

        if (device->hasFeature("poll_rate")) {
            qDebug() << "Set_pollrate:" << device->setPollRate(125);
            qDebug() << "Pollrate:" << device->getPollRate();
            qDebug() << "Set_pollrate:" << device->setPollRate(1000);
            qDebug() << "Pollrate:" << device->getPollRate();
        }

        foreach (libopenrazer::Led *led, device->getLeds()) {
            if (led->hasFx("brightness")) {
                qDebug() << "getBrightness";
                qDebug() << led->getBrightness();
            }
            auto colors = led->getCurrentColors();
            qDebug() << colors[0].r << colors[0].g << colors[0].b;
        }

        if (device->hasFeature("kbd_layout")) {
            qDebug() << "Keyboard layout:";
            qDebug() << device->getKeyboardLayout();
        }

        // if(device->hasMatrix()) {
        //     QList<int> dimen = device->getMatrixDimensions();
        //     qDebug() << dimen;
        //     qDebug() << dimen[0] << "-" << dimen[1];
        //     QList<QColor> colors = QList<QColor>();
        //     for(int i=0; i<dimen[1]; i++)
        //         colors << QColor("yellow");
        //     qDebug() << "size:" << colors.size();
        //     for(int i=0; i<dimen[0]; i++) {
        //         qDebug() << i;
        //         device->setKeyRow(i, 0, dimen[1]-1, colors);
        //         device->setCustom();
        //         qDebug() << "Press Enter to continue.";
        //         std::cin.ignore();
        //     }
        // }

        // QHash<QString, bool> hash = device->getAllCapabilities();
        // for (QHash<QString, bool>::iterator i = hash.begin(); i != hash.end(); ++i)
        //     qDebug() << i.key() << ": " << i.value();

        delete device;
    }
}
