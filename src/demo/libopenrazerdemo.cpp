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
    // qDebug() << "Supported devices:" << manager->getSupportedDevices();
    manager->syncEffects(false);

    foreach (const QDBusObjectPath &devicePath, manager->getDevices()) {
        qDebug() << "-----------------";
        libopenrazer::Device *device = manager->getDevice(devicePath);
        qDebug() << "Device name:" << device->getDeviceName();
        qDebug() << "Firmware version:" << device->getFirmwareVersion();
        try { // fake driver doesn't have device_mode
            qDebug() << "Device mode:" << device->getDeviceMode();
        } catch (const QException &e) {
        }
        // device->setDeviceMode(0x03, 0x00);
        // qDebug() << "Devicemode:" << device->getDeviceMode();
        qDebug() << "Serial:" << device->getSerial();

        if (device->hasFeature("dpi")) {
            openrazer::RazerDPI dpi = device->getDPI();
            qDebug() << "DPI:" << dpi;
            // FIXME Devices use 'available_dpi'
            if (device->getDeviceName() == "Razer DeathAdder 3.5G Black" ||
                device->getDeviceName() == "Razer DeathAdder 3.5G") {
                device->setDPI({ 900, 0 });
            } else {
                device->setDPI({ 500, 500 });
            }
            qDebug() << "DPI:" << device->getDPI();
            qDebug() << "Maximum DPI:" << device->maxDPI();
            // restore DPI
            device->setDPI(dpi);
        }

        if (device->hasFeature("poll_rate")) {
            ushort poll_rate = device->getPollRate();
            qDebug() << "Poll rate:" << poll_rate;
            device->setPollRate(125);
            qDebug() << "Poll rate:" << device->getPollRate();
            // restore poll rate
            device->setPollRate(poll_rate);
        }

        foreach (libopenrazer::Led *led, device->getLeds()) {
            qDebug() << "LED:" << led->getLedId();
            if (led->hasBrightness()) {
                qDebug() << "  Brightness:" << led->getBrightness();
            }
            qDebug() << "  Effect:" << led->getCurrentEffect();
            auto colors = led->getCurrentColors();
            qDebug() << "  Colors:" << colors;
        }

        if (device->hasFeature("keyboard_layout")) {
            qDebug() << "Keyboard layout:" << device->getKeyboardLayout();
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
