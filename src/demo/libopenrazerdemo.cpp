#include "libopenrazer.h"

#include <QColor>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

void setEffect(libopenrazer::Led *led, openrazer::RazerEffect effect, QVector<QColor> colors)
{
    if (colors.size() < 2)
        colors += QColor(Qt::lightGray);
    if (colors.size() < 2)
        colors += QColor(Qt::magenta);

    switch (effect) {
    case openrazer::RazerEffect::Off: {
        led->setOff();
        break;
    }
    case openrazer::RazerEffect::On: {
        led->setOn();
        break;
    }
    case openrazer::RazerEffect::Static: {
        led->setStatic(colors[0]);
        break;
    }
    case openrazer::RazerEffect::Breathing: {
        led->setBreathing(colors[0]);
        break;
    }
    case openrazer::RazerEffect::BreathingDual: {
        led->setBreathingDual(colors[0], colors[1]);
        break;
    }
    case openrazer::RazerEffect::BreathingRandom: {
        led->setBreathingRandom();
        break;
    }
    case openrazer::RazerEffect::Blinking: {
        led->setBlinking(colors[0]);
        break;
    }
    case openrazer::RazerEffect::Spectrum: {
        led->setSpectrum();
        break;
    }
    case openrazer::RazerEffect::Wave: {
        led->setWave(openrazer::WaveDirection::LEFT_TO_RIGHT);
        break;
    }
    case openrazer::RazerEffect::Reactive: {
        led->setReactive(colors[0], openrazer::ReactiveSpeed::_500MS);
        break;
    }
    default:
        throw new std::invalid_argument("Effect not handled: " + QVariant::fromValue(effect).toString().toStdString());
    }
}

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
    qDebug() << "Supported devices:" << manager->getSupportedDevices().size();
    bool syncEffects = manager->getSyncEffects();
    qDebug() << "Sync effects:" << syncEffects;
    manager->syncEffects(false);
    bool screensaver = manager->getTurnOffOnScreensaver();
    qDebug() << "Turn off on screensaver:" << screensaver;
    manager->setTurnOffOnScreensaver(false);

    for (const QDBusObjectPath &devicePath : manager->getDevices()) {
        qDebug() << "-----------------";
        libopenrazer::Device *device = manager->getDevice(devicePath);
        qDebug() << "Device name:" << device->getDeviceName();
        qDebug() << "Serial:" << device->getSerial();
        qDebug() << "Firmware version:" << device->getFirmwareVersion();
        qDebug() << "Device mode:" << device->getDeviceMode();
        qDebug() << "Device type:" << device->getDeviceType();
        qDebug() << "Device image:" << device->getDeviceImageUrl();

        if (device->hasFeature("keyboard_layout")) {
            qDebug() << "Keyboard layout:" << device->getKeyboardLayout();
        }

        if (device->hasFeature("dpi")) {
            openrazer::RazerDPI dpi = device->getDPI();
            qDebug() << "DPI:" << dpi;
            // FIXME Devices use 'available_dpi'
            if (device->getDeviceName() == "Razer DeathAdder 3.5G Black"
                || device->getDeviceName() == "Razer DeathAdder 3.5G") {
                device->setDPI({ 900, 0 });
            } else {
                device->setDPI({ 500, 500 });
            }
            qDebug() << "Maximum DPI:" << device->maxDPI();
            // restore DPI
            device->setDPI(dpi);
        }

        if (device->hasFeature("poll_rate")) {
            ushort poll_rate = device->getPollRate();
            qDebug() << "Poll rate:" << poll_rate;
            device->setPollRate(125);
            // restore poll rate
            device->setPollRate(poll_rate);
        }

        if (device->hasFeature("custom_frame")) {
            qDebug() << "Matrix dimensions:" << device->getMatrixDimensions();
        }

        for (libopenrazer::Led *led : device->getLeds()) {
            qDebug() << "LED:" << led->getLedId();
            if (led->hasBrightness()) {
                qDebug() << "  Brightness:" << led->getBrightness();
            }
            openrazer::RazerEffect effect = led->getCurrentEffect();
            qDebug() << "  Effect:" << effect;
            QVector<openrazer::RGB> colors = led->getCurrentColors();
            qDebug() << "  Colors:" << colors;

            for (const libopenrazer::RazerCapability &cap : libopenrazer::ledFxList) {
                if (led->hasFx(cap.getIdentifier())) {
                    setEffect(led, cap.getIdentifier(), QVector<QColor>());
                }
            }

            // restore effect
            if (!led->hasFx(effect)) {
                qDebug() << "  WARN: Cannot set effect from getCurrentEffect" << effect;
                continue;
            }
            // TODO: Use 'colors' variable to restore colors
            setEffect(led, effect, QVector<QColor>());
        }

        delete device;
    }

    // restore settings
    manager->setTurnOffOnScreensaver(screensaver);
    manager->syncEffects(syncEffects);
}
