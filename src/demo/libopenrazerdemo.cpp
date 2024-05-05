// Copyright (C) 2018  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libopenrazer.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

#include <stdexcept>

void setEffect(libopenrazer::Led *led, openrazer::Effect effect, QVector<::openrazer::RGB> colors)
{
    if (colors.size() < 2)
        colors += ::openrazer::RGB { 50, 254, 100 };
    if (colors.size() < 2)
        colors += ::openrazer::RGB { 173, 16, 160 };

    switch (effect) {
    case openrazer::Effect::Off: {
        led->setOff();
        break;
    }
    case openrazer::Effect::On: {
        led->setOn();
        break;
    }
    case openrazer::Effect::Static: {
        led->setStatic(colors[0]);
        break;
    }
    case openrazer::Effect::Breathing: {
        led->setBreathing(colors[0]);
        break;
    }
    case openrazer::Effect::BreathingDual: {
        led->setBreathingDual(colors[0], colors[1]);
        break;
    }
    case openrazer::Effect::BreathingRandom: {
        led->setBreathingRandom();
        break;
    }
    case openrazer::Effect::BreathingMono: {
        led->setBreathingMono();
        break;
    }
    case openrazer::Effect::Blinking: {
        led->setBlinking(colors[0]);
        break;
    }
    case openrazer::Effect::Spectrum: {
        led->setSpectrum();
        break;
    }
    case openrazer::Effect::Wave: {
        led->setWave(openrazer::WaveDirection::LEFT_TO_RIGHT);
        break;
    }
    case openrazer::Effect::Wheel: {
        led->setWheel(openrazer::WheelDirection::CLOCKWISE);
        break;
    }
    case openrazer::Effect::Reactive: {
        led->setReactive(colors[0], openrazer::ReactiveSpeed::_500MS);
        break;
    }
    case openrazer::Effect::Ripple: {
        led->setRipple(colors[0]);
        break;
    }
    case openrazer::Effect::RippleRandom: {
        led->setRippleRandom();
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
            openrazer::DPI dpi = device->getDPI();
            qDebug() << "DPI:" << dpi;
            if (device->hasFeature("restricted_dpi")) {
                QVector<ushort> allowedDPI = device->getAllowedDPI();
                qDebug() << "Allowed DPI:" << allowedDPI;
                device->setDPI({ allowedDPI.first(), 0 });
                device->setDPI({ allowedDPI.last(), 0 });
            } else {
                device->setDPI({ 500, 500 });
            }
            if (device->hasFeature("dpi_stages")) {
                QPair<uchar, QVector<openrazer::DPI>> dpiStages = device->getDPIStages();
                qDebug() << "DPI stages:" << dpiStages;
                device->setDPIStages(2, { { 400, 500 }, { 600, 700 }, { 800, 900 } });
                // restore DPI stages
                device->setDPIStages(dpiStages.first, dpiStages.second);
            }
            qDebug() << "Maximum DPI:" << device->maxDPI();
            // restore DPI
            device->setDPI(dpi);
        }

        if (device->hasFeature("poll_rate")) {
            ushort poll_rate = device->getPollRate();
            qDebug() << "Poll rate:" << poll_rate;
            QVector<ushort> supportedPollRates = device->getSupportedPollRates();
            qDebug() << "Supported poll rates:" << supportedPollRates;
            device->setPollRate(supportedPollRates.first());
            device->setPollRate(supportedPollRates.last());
            // restore poll rate
            device->setPollRate(poll_rate);
        }

        if (device->hasFeature("custom_frame")) {
            qDebug() << "Matrix dimensions:" << device->getMatrixDimensions();
        }

        if (device->hasFeature("battery")) {
            qDebug() << "Battery:" << device->getBatteryPercent() << "%";
            qDebug() << "Charging:" << device->isCharging();
        }

        if (device->hasFeature("low_battery_threshold")) {
            double threshold = device->getLowBatteryThreshold();
            qDebug() << "Low battery threshold:" << threshold << "%";
            device->setLowBatteryThreshold(50);
            // restore low battery threshold
            device->setLowBatteryThreshold(threshold);
        }

        if (device->hasFeature("idle_time")) {
            ushort idleTime = device->getIdleTime();
            qDebug() << "Idle time:" << idleTime << "seconds";
            device->setIdleTime(900);
            // restore idle time
            device->setIdleTime(idleTime);
        }

        for (libopenrazer::Led *led : device->getLeds()) {
            qDebug() << "LED:" << led->getLedId();
            if (led->hasBrightness()) {
                qDebug() << "  Brightness:" << led->getBrightness();
            }
            openrazer::Effect effect = led->getCurrentEffect();
            qDebug() << "  Effect:" << effect;
            QVector<openrazer::RGB> colors = led->getCurrentColors();
            qDebug() << "  Colors:" << colors;
            openrazer::WaveDirection waveDirection = led->getWaveDirection();
            qDebug() << "  Wave direction:" << waveDirection;

            for (const libopenrazer::Capability &cap : libopenrazer::ledFxList) {
                if (led->hasFx(cap.getIdentifier())) {
                    setEffect(led, cap.getIdentifier(), QVector<::openrazer::RGB>());
                }
            }

            // restore effect
            if (!led->hasFx(effect)) {
                qDebug() << "  WARN: Cannot set effect from getCurrentEffect" << effect;
                continue;
            }
            setEffect(led, effect, colors);
        }

        delete device;
    }

    // restore settings
    manager->setTurnOffOnScreensaver(screensaver);
    manager->syncEffects(syncEffects);
}
