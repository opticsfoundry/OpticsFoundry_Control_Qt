// ControlAPI.cpp: implementation of the API.
//
//////////////////////////////////////////////////////////////////////

#include "ControlAPI.h"

#include <QMessageBox>

#include <QDebug>
#include <QString>
#include <QDateTime>
//#include <QCoreApplication>

//#define EnableDebug

#ifndef EnableDebug
#define SlowDLLAccessToImproveStability
#endif

#ifdef SlowDLLAccessToImproveStability
#include <QThread>
#endif

void ErrorNotYetImplemented() {
    QMessageBox msgBox;
    msgBox.setText("ControlAPI.cpp: function not yet implemented");
    msgBox.exec();
}

CControlAPI::CControlAPI()
{
    ConnectedToLowLevelSoftware = false;
    telnet = NULL;
    DebugFileDirectory="";
    DebugFile= nullptr;
    DebugTextStream = nullptr;
#ifdef USE_CA_DLL
    Set_CA_DLL_CallsToNull();
#endif
}

bool CControlAPI::isConnected(bool no_error_message) const {

#ifdef USE_CA_DLL
    return true;
#else
    if (!telnet) return false;
    else return telnet->isConnected(no_error_message);
#endif
}

unsigned int CControlAPI::GetNumberReconnects() const {
#ifdef USE_CA_DLL
    return 0;
#else
    if (!telnet) return 0;
    else return telnet->NumberReconnects;
#endif
}


bool CControlAPI::UsingDLL() const {
#ifdef USE_CA_DLL
    return true;
#else
    return false;
#endif
}

#ifdef USE_CA_DLL

void CControlAPI::Set_CA_DLL_CallsToNull() {
    CA_DLL_Handle = NULL;
    CA_DLL_GetInstance = NULL;
    CA_DLL_Create = NULL;
    CA_DLL_Configure = NULL;
    CA_DLL_Command = NULL;
    CA_DLL_DidCommandErrorOccur = NULL;
    CA_DLL_GetLastError = NULL;
    CA_DLL_StoreSequenceInMemory = NULL;
    CA_DLL_SwitchToDirectOutputMode = NULL;
    CA_DLL_StartSequence = NULL;
    CA_DLL_IsSequenceRunning = NULL;
    CA_DLL_GetLastCommandLineNumber = NULL;
    CA_DLL_WaitTillSequenceEnds = NULL;
    CA_DLL_ResetFPGA = NULL;
    CA_DLL_ConnectToSequencer = NULL;
    CA_DLL_CheckIfSequencerReady = NULL;
    CA_DLL_ProgramSequence = NULL;
    CA_DLL_ProgramInterlockSequence = NULL;
    CA_DLL_ReplaceCommand = NULL;
    CA_DLL_ReplaceCommandForNextCycle = NULL;
    CA_DLL_ReplaceCommandsForNextCycle = NULL;
    CA_DLL_ResetCommandList = NULL;
    CA_DLL_AssembleSequenceListFromMemory = NULL;
    CA_DLL_StartCycling = NULL;
    CA_DLL_StopCycling = NULL;
    CA_DLL_IsCycling = NULL;
    CA_DLL_DataAvailable = NULL;
    CA_DLL_GetNextCycleStartTimeAndNumber = NULL;
    CA_DLL_ResetCycleNumber = NULL;
    CA_DLL_InterruptSequence = NULL;
    CA_DLL_WriteReadSPI = NULL;
    CA_DLL_WaitTillEndOfSequenceThenGetInputData = NULL;
    CA_DLL_GetCycleData = NULL;
    CA_DLL_ClearAnalogInputQueue = NULL;
    CA_DLL_HasInterlockTriggered = NULL;
    CA_DLL_ResetInterlock = NULL;
    CA_DLL_SetExternalTrigger = NULL;
    CA_DLL_SetPeriodicTrigger = NULL;
    CA_DLL_GetPeriodicTriggerError = NULL;
    CA_DLL_SetExternalClock = NULL;
    CA_DLL_SetupSerialPort = NULL;
    CA_DLL_WriteToSerial = NULL;
    CA_DLL_WriteToI2C = NULL;
    CA_DLL_WriteToSPI = NULL;
    CA_DLL_GetSequenceDuration = NULL;
    CA_DLL_GetTimeInMs = NULL;
    CA_DLL_Ramp = NULL;
    CA_DLL_Wait = NULL;
    CA_DLL_WaitTillBusBufferEmpty = NULL;
    CA_DLL_WaitTillRampsEnd = NULL;
    CA_DLL_StopRamps = NULL;
    CA_DLL_DoNothing = NULL;
    CA_DLL_StartAnalogInAcquisition = NULL;
    CA_DLL_StartXADCAnalogInAcquisition = NULL;
    CA_DLL_GoBackInTime = NULL;
    CA_DLL_GoToTime = NULL;
    CA_DLL_ReturnToCurrentTime = NULL;
    CA_DLL_FinishLastGoBackInTime = NULL;
    CA_DLL_WriteInputMemory = NULL;
    CA_DLL_WriteSystemTimeToInputMemory = NULL;
    CA_DLL_SwitchDebugLED = NULL;
    CA_DLL_IgnoreTCPIP = NULL;
    CA_DLL_AddMarker = NULL;
}
#endif


CControlAPI::~CControlAPI()
{
    DisconnectFromLowLevelSoftware();
    if (DebugTextStream) {
        DebugTextStream->flush();
        delete DebugTextStream;
        DebugTextStream = nullptr;
    }
    if (DebugFile) {
        DebugFile->close();
        delete DebugFile;
        DebugFile = nullptr;
    }
}

void CControlAPI::MessageBox(QString aMessage) {
    QMessageBox msgBox;
    msgBox.setText(aMessage);
    msgBox.exec();
}

#ifdef USE_CA_DLL

bool CControlAPI::ConnectToLowLevelSoftware(QTelnet * atelnet, const char* ParamFileName, const char* IP,  const bool Debug, const QString _DebugFileDirectory, unsigned long port, double timeout_in_seconds) {

    
    CA_DLL_Lib = new QLibrary("Control.dll");

    if (!CA_DLL_Lib->load()) {
        qDebug() << "Failed to load DLL:" << CA_DLL_Lib->errorString();
        return -1;
    }

    /*ChatGPT, load the following functions:
API_EXPORT HControlAPI ControlAPI_GetInstance();
API_EXPORT bool ControlAPI_Configure(bool displayErrors);
API_EXPORT void ControlAPI_Cleanup();
API_EXPORT long ControlAPI_Command(const char* code);
API_EXPORT bool ControlAPI_DidCommandErrorOccur(long* lineNumber, const char** commandWithError);
API_EXPORT const char* ControlAPI_GetLastError();
 API_EXPORT void ControlAPI_StoreSequenceInMemory(bool store);
    API_EXPORT void ControlAPI_SwitchToDirectOutputMode();
    API_EXPORT bool ControlAPI_StartSequence(bool showDialog);
    API_EXPORT bool ControlAPI_IsSequenceRunning();
    API_EXPORT long ControlAPI_GetLastCommandLineNumber();
    API_EXPORT bool ControlAPI_WaitTillSequenceEnds(double timeout);
API_EXPORT bool ControlAPI_ResetFPGA();
API_EXPORT bool ControlAPI_ConnectToSequencer(const unsigned char* IP, unsigned long port, double timeout);
API_EXPORT bool ControlAPI_CheckIfSequencerReady(double timeout);
API_EXPORT void ControlAPI_ProgramSequence();
API_EXPORT bool ControlAPI_ProgramInterlockSequence();
API_EXPORT bool ControlAPI_ReplaceCommand(unsigned long cycle_number, unsigned int command_line_nr, const char* new_command);
API_EXPORT void ControlAPI_ReplaceCommandForNextCycle(unsigned int command_line_nr, const char* new_command);
API_EXPORT void ControlAPI_ReplaceCommandsForNextCycle();
API_EXPORT void ControlAPI_ResetCommandList();
API_EXPORT bool ControlAPI_AssembleSequenceListFromMemory();
API_EXPORT bool ControlAPI_StartCycling(long idleTime_ms, long softPreTrigger_ms, bool onlyTransmitDiff, bool showDialog);
API_EXPORT void ControlAPI_StopCycling();
API_EXPORT bool ControlAPI_IsCycling();
API_EXPORT bool ControlAPI_DataAvailable();
API_EXPORT bool ControlAPI_GetNextCycleStartTimeAndNumber(long* timeTillNextCycleStart_ms, long* nextCycleNumber);
API_EXPORT bool ControlAPI_ResetCycleNumber();
API_EXPORT bool ControlAPI_InterruptSequence();
API_EXPORT void ControlAPI_WriteReadSPI(unsigned int chip_select, unsigned int num_bits_out, unsigned long long data_high, unsigned long long data_low, unsigned int num_bits_in);
API_EXPORT bool ControlAPI_WaitTillEndOfSequenceThenGetInputData(unsigned char** buffer, unsigned long* buffer_length, unsigned long* endTimeOfCycle, double timeout);
API_EXPORT bool ControlAPI_GetCycleData(unsigned char** buffer, unsigned long* buffer_length, long* cycleNumber, unsigned long* lastCycleEndTime, unsigned long* lastCycleStartPreTriggerTime, bool* cycleError, const char** errorMessages);
API_EXPORT bool ControlAPI_ClearAnalogInputQueue();
API_EXPORT bool ControlAPI_HasInterlockTriggered();
API_EXPORT bool ControlAPI_ResetInterlock();
API_EXPORT void ControlAPI_SetExternalTrigger(bool externalTrigger0, bool externalTrigger1);
API_EXPORT void ControlAPI_SetPeriodicTrigger(double period_ms, double allowedWait_ms);
API_EXPORT bool ControlAPI_GetPeriodicTriggerError();
API_EXPORT void ControlAPI_SetExternalClock(bool externalClock0, bool externalClock1);
API_EXPORT bool ControlAPI_SetupSerialPort(unsigned char port_number, unsigned long baud_rate);
API_EXPORT bool ControlAPI_WriteToSerial(unsigned int port_nr, const char* command, unsigned long length);
API_EXPORT bool ControlAPI_WriteToI2C(unsigned int port_nr, const char* command, unsigned long length);
API_EXPORT bool ControlAPI_WriteToSPI(unsigned int port_nr, const char* command, unsigned long length);
API_EXPORT double ControlAPI_GetSequenceDuration();
API_EXPORT double ControlAPI_GetTimeInMs();
API_EXPORT void ControlAPI_Ramp(const char* output_name, double start_value, double end_value, double ramp_time_ms, double timestep_ms);
API_EXPORT void ControlAPI_Wait(double time_ms, unsigned long ID);
API_EXPORT void ControlAPI_WaitTillBusBufferEmpty(unsigned long ID);
API_EXPORT void ControlAPI_WaitTillRampsEnd(unsigned long ID);
API_EXPORT void ControlAPI_StopRamps();
API_EXPORT void ControlAPI_DoNothing();
API_EXPORT void ControlAPI_StartAnalogInAcquisition(unsigned char sequencer, unsigned char SPI_port, unsigned char SPI_CS, unsigned int channel_number, unsigned int num_datapoints, double delay_ms);
API_EXPORT void ControlAPI_StartXADCAnalogInAcquisition(unsigned int channel_number, unsigned int num_datapoints, double delay_ms);
API_EXPORT void ControlAPI_GoBackInTime(double timeJump_ms, unsigned int ID);
API_EXPORT void ControlAPI_GoToTime(double time_ms, unsigned int ID);
API_EXPORT void ControlAPI_ReturnToCurrentTime(unsigned int ID);
API_EXPORT void ControlAPI_FinishLastGoBackInTime(unsigned int ID);
API_EXPORT void ControlAPI_WriteInputMemory(unsigned long data, bool write_next_address, unsigned long address);
API_EXPORT void ControlAPI_WriteSystemTimeToInputMemory();
API_EXPORT void ControlAPI_SwitchDebugLED(unsigned int onOff);
API_EXPORT void ControlAPI_IgnoreTCPIP(bool onOff);
API_EXPORT void ControlAPI_AddMarker(unsigned char marker);

 */

    CA_DLL_GetInstance = (GetInstanceFunc)CA_DLL_Lib->resolve("ControlAPI_GetInstance");
    CA_DLL_Create = (CreateFunc)CA_DLL_Lib->resolve("ControlAPI_Create");
    CA_DLL_Configure = (ConfigureFunc)CA_DLL_Lib->resolve("ControlAPI_Configure");
    CA_DLL_Cleanup = (CleanupFunc)CA_DLL_Lib->resolve("ControlAPI_Cleanup");
    CA_DLL_Command = (CommandFunc)CA_DLL_Lib->resolve("ControlAPI_Command");
    CA_DLL_DidCommandErrorOccur = (DidErrorFunc)CA_DLL_Lib->resolve("ControlAPI_DidCommandErrorOccur");
    CA_DLL_GetLastError = (GetErrorFunc)CA_DLL_Lib->resolve("ControlAPI_GetLastError");
    CA_DLL_StoreSequenceInMemory = (StoreSequenceInMemoryFunc)CA_DLL_Lib->resolve("ControlAPI_StoreSequenceInMemory");
    CA_DLL_SwitchToDirectOutputMode = (SwitchToDirectOutputModeFunc)CA_DLL_Lib->resolve("ControlAPI_SwitchToDirectOutputMode");
    CA_DLL_StartSequence = (StartSequenceFunc)CA_DLL_Lib->resolve("ControlAPI_StartSequence");
    CA_DLL_IsSequenceRunning = (IsSequenceRunningFunc)CA_DLL_Lib->resolve("ControlAPI_IsSequenceRunning");
    CA_DLL_GetLastCommandLineNumber = (GetLastCommandLineNumberFunc)CA_DLL_Lib->resolve("ControlAPI_GetLastCommandLineNumber");
    CA_DLL_WaitTillSequenceEnds = (WaitTillSequenceEndsFunc)CA_DLL_Lib->resolve("ControlAPI_WaitTillSequenceEnds");
    CA_DLL_ResetFPGA = (ResetFPGAFunc)CA_DLL_Lib->resolve("ControlAPI_ResetFPGA");
    CA_DLL_ConnectToSequencer = (ConnectToSequencerFunc)CA_DLL_Lib->resolve("ControlAPI_ConnectToSequencer");
    CA_DLL_CheckIfSequencerReady = (CheckIfSequencerReadyFunc)CA_DLL_Lib->resolve("ControlAPI_CheckIfSequencerReady");
    CA_DLL_ProgramSequence = (ProgramSequenceFunc)CA_DLL_Lib->resolve("ControlAPI_ProgramSequence");
    CA_DLL_ProgramInterlockSequence = (ProgramInterlockSequenceFunc)CA_DLL_Lib->resolve("ControlAPI_ProgramInterlockSequence");
    CA_DLL_ReplaceCommand = (ReplaceCommandFunc)CA_DLL_Lib->resolve("ControlAPI_ReplaceCommand");
    CA_DLL_ReplaceCommandForNextCycle = (ReplaceCommandForNextCycleFunc)CA_DLL_Lib->resolve("ControlAPI_ReplaceCommandForNextCycle");
    CA_DLL_ReplaceCommandsForNextCycle = (ReplaceCommandsForNextCycleFunc)CA_DLL_Lib->resolve("ControlAPI_ReplaceCommandsForNextCycle");
    CA_DLL_ResetCommandList = (ResetCommandListFunc)CA_DLL_Lib->resolve("ControlAPI_ResetCommandList");
    CA_DLL_AssembleSequenceListFromMemory = (AssembleSequenceListFromMemoryFunc)CA_DLL_Lib->resolve("ControlAPI_AssembleSequenceListFromMemory");
    CA_DLL_StartCycling = (StartCyclingFunc)CA_DLL_Lib->resolve("ControlAPI_StartCycling");
    CA_DLL_StopCycling = (StopCyclingFunc)CA_DLL_Lib->resolve("ControlAPI_StopCycling");
    CA_DLL_IsCycling = (IsCyclingFunc)CA_DLL_Lib->resolve("ControlAPI_IsCycling");
    CA_DLL_DataAvailable = (DataAvailableFunc)CA_DLL_Lib->resolve("ControlAPI_DataAvailable");
    CA_DLL_GetNextCycleStartTimeAndNumber = (GetNextCycleStartTimeAndNumberFunc)CA_DLL_Lib->resolve("ControlAPI_GetNextCycleStartTimeAndNumber");
    CA_DLL_ResetCycleNumber = (ResetCycleNumberFunc)CA_DLL_Lib->resolve("ControlAPI_ResetCycleNumber");
    CA_DLL_InterruptSequence = (InterruptSequenceFunc)CA_DLL_Lib->resolve("ControlAPI_InterruptSequence");
    CA_DLL_WriteReadSPI = (WriteReadSPIFunc)CA_DLL_Lib->resolve("ControlAPI_WriteReadSPI");
    CA_DLL_WaitTillEndOfSequenceThenGetInputData = (WaitTillEndOfSequenceThenGetInputDataFunc)CA_DLL_Lib->resolve("ControlAPI_WaitTillEndOfSequenceThenGetInputData");
    CA_DLL_GetCycleData = (GetCycleDataFunc)CA_DLL_Lib->resolve("ControlAPI_GetCycleData");
    CA_DLL_ClearAnalogInputQueue = (ClearAnalogInputQueueFunc)CA_DLL_Lib->resolve("ControlAPI_ClearAnalogInputQueue");
    CA_DLL_HasInterlockTriggered = (HasInterlockTriggeredFunc)CA_DLL_Lib->resolve("ControlAPI_HasInterlockTriggered");
    CA_DLL_ResetInterlock = (ResetInterlockFunc)CA_DLL_Lib->resolve("ControlAPI_ResetInterlock");
    CA_DLL_SetExternalTrigger = (SetExternalTriggerFunc)CA_DLL_Lib->resolve("ControlAPI_SetExternalTrigger");
    CA_DLL_SetPeriodicTrigger = (SetPeriodicTriggerFunc)CA_DLL_Lib->resolve("ControlAPI_SetPeriodicTrigger");
    CA_DLL_GetPeriodicTriggerError = (GetPeriodicTriggerErrorFunc)CA_DLL_Lib->resolve("ControlAPI_GetPeriodicTriggerError");
    CA_DLL_SetExternalClock = (SetExternalClockFunc)CA_DLL_Lib->resolve("ControlAPI_SetExternalClock");
    CA_DLL_SetupSerialPort = (SetupSerialPortFunc)CA_DLL_Lib->resolve("ControlAPI_SetupSerialPort");
    CA_DLL_WriteToSerial = (WriteToSerialFunc)CA_DLL_Lib->resolve("ControlAPI_WriteToSerial");
    CA_DLL_WriteToI2C = (WriteToI2CFunc)CA_DLL_Lib->resolve("ControlAPI_WriteToI2C");
    CA_DLL_WriteToSPI = (WriteToSPIFunc)CA_DLL_Lib->resolve("ControlAPI_WriteToSPI");
    CA_DLL_GetSequenceDuration = (GetSequenceDurationFunc)CA_DLL_Lib->resolve("ControlAPI_GetSequenceDuration");
    CA_DLL_GetTimeInMs = (GetTimeInMsFunc)CA_DLL_Lib->resolve("ControlAPI_GetTimeInMs");
    CA_DLL_Ramp = (RampFunc)CA_DLL_Lib->resolve("ControlAPI_Ramp");
    CA_DLL_Wait = (WaitFunc)CA_DLL_Lib->resolve("ControlAPI_Wait");
    CA_DLL_WaitTillBusBufferEmpty = (WaitTillBusBufferEmptyFunc)CA_DLL_Lib->resolve("ControlAPI_WaitTillBusBufferEmpty");
    CA_DLL_WaitTillRampsEnd = (WaitTillRampsEndFunc)CA_DLL_Lib->resolve("ControlAPI_WaitTillRampsEnd");
    CA_DLL_StopRamps = (StopRampsFunc)CA_DLL_Lib->resolve("ControlAPI_StopRamps");
    CA_DLL_DoNothing = (DoNothingFunc)CA_DLL_Lib->resolve("ControlAPI_DoNothing");
    CA_DLL_StartAnalogInAcquisition = (StartAnalogInAcquisitionFunc)CA_DLL_Lib->resolve("ControlAPI_StartAnalogInAcquisition");
    CA_DLL_StartXADCAnalogInAcquisition = (StartXADCAnalogInAcquisitionFunc)CA_DLL_Lib->resolve("ControlAPI_StartXADCAnalogInAcquisition");
    CA_DLL_GoBackInTime = (GoBackInTimeFunc)CA_DLL_Lib->resolve("ControlAPI_GoBackInTime");
    CA_DLL_GoToTime = (GoToTimeFunc)CA_DLL_Lib->resolve("ControlAPI_GoToTime");
    CA_DLL_ReturnToCurrentTime = (ReturnToCurrentTimeFunc)CA_DLL_Lib->resolve("ControlAPI_ReturnToCurrentTime");
    CA_DLL_FinishLastGoBackInTime = (FinishLastGoBackInTimeFunc)CA_DLL_Lib->resolve("ControlAPI_FinishLastGoBackInTime");
    CA_DLL_WriteInputMemory = (WriteInputMemoryFunc)CA_DLL_Lib->resolve("ControlAPI_WriteInputMemory");
    CA_DLL_WriteSystemTimeToInputMemory = (WriteSystemTimeToInputMemoryFunc)CA_DLL_Lib->resolve("ControlAPI_WriteSystemTimeToInputMemory");
    CA_DLL_SwitchDebugLED = (SwitchDebugLEDFunc)CA_DLL_Lib->resolve("ControlAPI_SwitchDebugLED");
    CA_DLL_IgnoreTCPIP = (IgnoreTCPIPFunc)CA_DLL_Lib->resolve("ControlAPI_IgnoreTCPIP");
    CA_DLL_AddMarker = (AddMarkerFunc)CA_DLL_Lib->resolve("ControlAPI_AddMarker");


    if (
        !CA_DLL_GetInstance ||
        !CA_DLL_Create ||
        !CA_DLL_Configure ||
        !CA_DLL_Command ||
        !CA_DLL_DidCommandErrorOccur ||
        !CA_DLL_GetLastError ||
        !CA_DLL_StoreSequenceInMemory ||
        !CA_DLL_SwitchToDirectOutputMode ||
        !CA_DLL_StartSequence ||
        !CA_DLL_IsSequenceRunning ||
        !CA_DLL_GetLastCommandLineNumber ||
        !CA_DLL_WaitTillSequenceEnds ||
        !CA_DLL_ResetFPGA ||
        !CA_DLL_ConnectToSequencer ||
        !CA_DLL_CheckIfSequencerReady ||
        !CA_DLL_ProgramSequence ||
        !CA_DLL_ProgramInterlockSequence ||
        !CA_DLL_ReplaceCommand ||
        !CA_DLL_ReplaceCommandForNextCycle ||
        !CA_DLL_ReplaceCommandsForNextCycle ||
        !CA_DLL_ResetCommandList ||
        !CA_DLL_AssembleSequenceListFromMemory ||
        !CA_DLL_StartCycling ||
        !CA_DLL_StopCycling ||
        !CA_DLL_IsCycling ||
        !CA_DLL_DataAvailable ||
        !CA_DLL_GetNextCycleStartTimeAndNumber ||
        !CA_DLL_ResetCycleNumber ||
        !CA_DLL_InterruptSequence ||
        !CA_DLL_WriteReadSPI ||
        !CA_DLL_WaitTillEndOfSequenceThenGetInputData ||
        !CA_DLL_GetCycleData ||
        !CA_DLL_ClearAnalogInputQueue ||
        !CA_DLL_HasInterlockTriggered ||
        !CA_DLL_ResetInterlock ||
        !CA_DLL_SetExternalTrigger ||
        !CA_DLL_SetPeriodicTrigger ||
        !CA_DLL_GetPeriodicTriggerError ||
        !CA_DLL_SetExternalClock ||
        !CA_DLL_SetupSerialPort ||
        !CA_DLL_WriteToSerial ||
        !CA_DLL_WriteToI2C ||
        !CA_DLL_WriteToSPI ||
        !CA_DLL_GetSequenceDuration ||
        !CA_DLL_GetTimeInMs ||
        !CA_DLL_Ramp ||
        !CA_DLL_Wait ||
        !CA_DLL_WaitTillBusBufferEmpty ||
        !CA_DLL_WaitTillRampsEnd ||
        !CA_DLL_StopRamps ||
        !CA_DLL_DoNothing ||
        !CA_DLL_StartAnalogInAcquisition ||
        !CA_DLL_StartXADCAnalogInAcquisition ||
        !CA_DLL_GoBackInTime ||
        !CA_DLL_GoToTime ||
        !CA_DLL_ReturnToCurrentTime ||
        !CA_DLL_FinishLastGoBackInTime ||
        !CA_DLL_WriteInputMemory ||
        !CA_DLL_WriteSystemTimeToInputMemory ||
        !CA_DLL_SwitchDebugLED ||
        !CA_DLL_IgnoreTCPIP ||
        !CA_DLL_AddMarker
        ) {
        qDebug() << "Failed to resolve one or more ControlAPI symbols.";
        DisconnectFromLowLevelSoftware();
        return -1;
    }
    DebugFileDirectory = _DebugFileDirectory;
    if (Debug && (DebugFileDirectory != "")) {
        if (DebugFile) {
            DebugFile->close();
            delete DebugFile;
            DebugFile = nullptr;
        }
        DebugFile = new QFile(DebugFileDirectory + "\\ControlAPI_Debug.txt");
        if (DebugFile->open(QIODevice::WriteOnly)) {
            DebugTextStream = new QTextStream(DebugFile);
            (*DebugTextStream) << "Start debugging ControlAPI at " << QDateTime::currentDateTime().toString() << "\n";
            (*DebugTextStream) << "IP: " << IP << ", Port: " << port << "\n";
            (*DebugTextStream) << "Timeout: " << timeout_in_seconds << " seconds\n";
            (*DebugTextStream) << "Using DLL: " << (UsingDLL() ? "Yes" : "No") << "\n";
            (*DebugTextStream) << "Connected to low-level software.\n";
            #ifdef EnableDebug
            (*DebugTextStream) << "Finegrained debugging enabled.\n";
            #else
            (*DebugTextStream) << "Finegrained debugging disabled. Enable it by defining EnableDebug in ControlAPI.cpp\n";
            #endif
            DebugTextStream->flush();
        } else {
            DebugFile->close();
            delete DebugFile;
            DebugFile = nullptr;
            MessageBox("Couldn't open file for writing");
        }
    }
    CA_DLL_Create(ParamFileName, true, true, true);
    CA_DLL_Handle = CA_DLL_GetInstance();
    ConnectedToLowLevelSoftware = true;
    return true;
}

#ifdef EnableDebug
#define WriteDebug(msg) \
    if (DebugTextStream) { \
        (*DebugTextStream) << msg; \
        DebugTextStream->flush(); \
    } 
#else
#ifdef SlowDLLAccessToImproveStability
    //QThread::usleep(10);//usleep(50); too slow -> use nsecsElapsed, despite being busy wait
#define WriteDebug(msg) \
    { \
    QElapsedTimer t; \
    t.start();\
    while (t.nsecsElapsed() < 5000); \
    }
#else
#define WriteDebug(msg)
#endif
#endif



void CControlAPI::DisconnectFromLowLevelSoftware() {
    if (CA_DLL_Lib) {
        Set_CA_DLL_CallsToNull();
        CA_DLL_Cleanup();
        CA_DLL_Cleanup = NULL;
        CA_DLL_Lib->unload();
        delete CA_DLL_Lib;
        CA_DLL_Lib = NULL;

    }
    ConnectedToLowLevelSoftware = false;
}

void CControlAPI::ConfigureControlAPI(bool DisplayCommandErrors) {
    WriteDebug("CCA")
    if (CA_DLL_Configure)
        CA_DLL_Configure(DisplayCommandErrors);
    WriteDebug("x ")
}

QString CControlAPI::GetError() {
    if (CA_DLL_GetLastError) {
        WriteDebug("GE")
        const char* error = CA_DLL_GetLastError();
        WriteDebug("x ")
        if (error) {
            return QString::fromUtf8(error);
        }
    }
    return QString();
}

void CControlAPI::StoreSequenceInMemory(bool DoStoreSequenceInMemory) {
    WriteDebug("SS")
    if (CA_DLL_StoreSequenceInMemory)
        CA_DLL_StoreSequenceInMemory(DoStoreSequenceInMemory);
    WriteDebug("x ")
}

bool CControlAPI::ConnectToSequencer(const char* IP, unsigned long port, double timeout_in_seconds) {
    if (CA_DLL_ConnectToSequencer) {
        WriteDebug("CTS")
        bool ret = CA_DLL_ConnectToSequencer(reinterpret_cast<const unsigned char*>(IP), port, timeout_in_seconds);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::CheckIfLowLevelSoftwareReady(double /*timeout_in_seconds*/) {
    return ConnectedToLowLevelSoftware;
}

bool CControlAPI::CheckIfSequencerReady(double timeout_in_seconds) {
    if (CA_DLL_CheckIfSequencerReady) {
        WriteDebug("CISR")
        bool ret = CA_DLL_CheckIfSequencerReady(timeout_in_seconds);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

long CControlAPI::Command(const char* command) {
    if (CA_DLL_Command) {
        WriteDebug("C")
        long ret = CA_DLL_Command(command);
        WriteDebug("x ")
        return ret;
    }
    return -1; // Indicate failure if the function is not available
}

bool CControlAPI::DidCommandErrorOccur(long& ErrorLineNr, QString& CodeWithError, double /*timeout_in_seconds*/) {
    if (CA_DLL_DidCommandErrorOccur) {
        long lineNr = 0;
        const char* errStr = nullptr;
        WriteDebug("DCEO")
        bool res = CA_DLL_DidCommandErrorOccur(&lineNr, &errStr);
        WriteDebug("x ")
        ErrorLineNr = lineNr;
        CodeWithError = errStr ? QString::fromUtf8(errStr) : QString();
        return res;
    }
    return false;
}

long CControlAPI::GetLastCommandLineNumber() {
    if (CA_DLL_GetLastCommandLineNumber) {
        WriteDebug("GL")
        long ret = CA_DLL_GetLastCommandLineNumber();
        WriteDebug("x ")
        return ret;
    }
    return -1;
}

void CControlAPI::ProgramSequence() {
    WriteDebug("PS")
    if (CA_DLL_ProgramSequence)
        CA_DLL_ProgramSequence();
    WriteDebug("x ")
}

void CControlAPI::SwitchToDirectOutputMode() {
    WriteDebug("STD")
    if (CA_DLL_SwitchToDirectOutputMode)
        CA_DLL_SwitchToDirectOutputMode();
    WriteDebug("x ")
}

bool CControlAPI::ProgramInterlockSequence() {
    if (CA_DLL_ProgramInterlockSequence) {
        WriteDebug("PIS")
        bool ret = CA_DLL_ProgramInterlockSequence();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::ReplaceCommand(unsigned long cycle_number, unsigned int command_line_nr, const char* new_command) {
    
    if (CA_DLL_ReplaceCommand) {
        WriteDebug("RC")
        CA_DLL_ReplaceCommand(cycle_number, command_line_nr, new_command);
        WriteDebug("x ")
    }
}

bool CControlAPI::StartSequence(bool ShowRunProgressDialog, double /*timeout_in_seconds*/) {
    if (CA_DLL_StartSequence) {
        WriteDebug("SS")
        bool ret = CA_DLL_StartSequence(ShowRunProgressDialog);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::ResetCycleNumber() {
    WriteDebug("RCN")
    if (CA_DLL_ResetCycleNumber) 
        CA_DLL_ResetCycleNumber();
    WriteDebug("x ")
}

bool CControlAPI::StartCycling(long readout_pre_trigger_in_ms, long soft_pre_trigger_in_ms, bool TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, bool diplay_progress_dialog) {
    if (CA_DLL_StartCycling) {
        WriteDebug("StartCycling")
        bool ret = CA_DLL_StartCycling(readout_pre_trigger_in_ms, soft_pre_trigger_in_ms, TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, diplay_progress_dialog);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::StopCycling() {
    WriteDebug("StopCycling")
    if (CA_DLL_StopCycling)
        CA_DLL_StopCycling();
    WriteDebug("x ")
}

bool CControlAPI::IsCycling(double /*timeout_in_seconds*/) {
    if (CA_DLL_IsCycling) {
        WriteDebug("IC")
        bool ret = CA_DLL_IsCycling();
        if (ret) {WriteDebug("1x ")}
        else {WriteDebug("0x ")}
        return ret;
    }
    return false;
}

bool CControlAPI::DataAvailable(double timeout_in_seconds) {
    if (CA_DLL_DataAvailable) {
        long WaitedForData_in_ms = 1;
        long Timeout_in_ms = timeout_in_seconds * 1000;
        WriteDebug("D1")
        bool DataAvailable = CA_DLL_DataAvailable();
        WriteDebug("x ")
        while ((!DataAvailable) && (WaitedForData_in_ms < Timeout_in_ms)) {
            Sleep_ms(10);
            WaitedForData_in_ms += 10;
            WriteDebug("D")
            DataAvailable = CA_DLL_DataAvailable();
            WriteDebug("x ")
        }
        return DataAvailable;
    }
    return false;
}

bool CControlAPI::GetNextCycleStartTimeAndNumber(long &TimeTillNextCycleStart_in_ms, long &NextCycleNumber, double /*timeout_in_seconds*/) {
    if (CA_DLL_GetNextCycleStartTimeAndNumber) {
        WriteDebug("GN")
        bool ret = CA_DLL_GetNextCycleStartTimeAndNumber(&TimeTillNextCycleStart_in_ms, &NextCycleNumber);
#ifdef EnableDebug
        if (ret) {
            QString text = QString::number(TimeTillNextCycleStart_in_ms)+ "ms " + QString::number(NextCycleNumber) + "x ";
            WriteDebug(text)
        }
        else {WriteDebug("0x ")}
#endif
        return ret;
    }
    return false;
}

bool CControlAPI::IsSequenceRunning() {
    if (CA_DLL_IsSequenceRunning) {
        WriteDebug("ISR")
        bool ret = CA_DLL_IsSequenceRunning();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::WaitTillSequenceEnds(double timeout_in_seconds) {
    if (CA_DLL_WaitTillSequenceEnds) {
        WriteDebug("WTSE")
        bool ret = CA_DLL_WaitTillSequenceEnds(timeout_in_seconds);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::InterruptSequence() {
    if (CA_DLL_InterruptSequence) {
        WriteDebug("IntS")
        bool ret = CA_DLL_InterruptSequence();
        WriteDebug("x ");
        return ret;
    }
    return false;
}

double CControlAPI::GetTime_in_ms() {
    if (CA_DLL_GetTimeInMs) {
        WriteDebug("GT")
        double ret = CA_DLL_GetTimeInMs();
        WriteDebug("x ")
        return ret;
    }
    return -1.0;
}

void CControlAPI::GoBackInTime(double aTimeJump_in_ms, unsigned int ID) {
    WriteDebug("GBIT")
    if (CA_DLL_GoBackInTime)
        CA_DLL_GoBackInTime(aTimeJump_in_ms, ID);
    WriteDebug("x ")
}

void CControlAPI::GoToTime(double aTime_in_ms, unsigned int ID) {
    WriteDebug("GTT")
    if (CA_DLL_GoToTime)
        CA_DLL_GoToTime(aTime_in_ms, ID);
    WriteDebug("x ")
}

void CControlAPI::ReturnToCurrentTime(unsigned int ID) {
    WriteDebug("RTCT")
    if (CA_DLL_ReturnToCurrentTime)
        CA_DLL_ReturnToCurrentTime(ID);
    WriteDebug("x ")
}

void CControlAPI::FinishLastGoBackInTime(unsigned int ID) {
    WriteDebug("FLGBIT")
    if (CA_DLL_FinishLastGoBackInTime)
        CA_DLL_FinishLastGoBackInTime(ID);
    WriteDebug("x ")
}

void CControlAPI::StartAnalogInAcquisition(unsigned char sequencer, unsigned char SPI_port, unsigned char SPI_CS, unsigned int channel_number, unsigned int number_of_datapoints, double delay_between_datapoints_in_ms) {
    WriteDebug("SAIA")
    if (CA_DLL_StartAnalogInAcquisition)
        CA_DLL_StartAnalogInAcquisition(sequencer, SPI_port, SPI_CS, channel_number, number_of_datapoints, delay_between_datapoints_in_ms);
    WriteDebug("x ")
}

bool CControlAPI::WaitTillEndOfSequenceThenGetInputData(unsigned int*& buffer, unsigned long& buffer_length, double timeout_in_seconds) {
    //
    //The buffer has been allocated by the DLL. It needs to be deleted by the DLL as well,
    //which is done the next time you call WaitTillEndOfSequenceThenGetInputData with the same buffer pointer.
   //
    if (CA_DLL_WaitTillEndOfSequenceThenGetInputData) {
        unsigned char* raw_buffer = nullptr;
        unsigned long buffer_length_in_bytes = 0;
        unsigned long dummy_end_time = 0;
        WriteDebug("WTSEGD")
        bool result = CA_DLL_WaitTillEndOfSequenceThenGetInputData(&raw_buffer, &buffer_length_in_bytes, &dummy_end_time, timeout_in_seconds);
        WriteDebug("x ")
        buffer = reinterpret_cast<unsigned int*>(raw_buffer);
        buffer_length = buffer_length_in_bytes/4;
        return result;
    }
    return false;
}

bool CControlAPI::GetCycleData(unsigned int*& buffer, unsigned long& buffer_length, long &CycleNumber, long &LastCycleEndTime, long &LastCycleStartPreTriggerTime, bool &CycleError, QString &ErrorMessages, double /*timeout_in_seconds*/) {
    //
    //The buffer has been allocated by the DLL. It needs to be deleted by the DLL as well and this will automatically be done
    //after a certain number of runs, currently set to 128 in ControlAPI.h of the DLLs source code.
    //The buffer is only valid for 128 cycles of the experiment. Afterwards the DLL will free the buffer's memory.
    //If the buffer is accessd after the DLL deleted it, the program will stall
    //If you intend to use the buffer's data for a long time, then create a copy of it into memory that you allocated.
    //
    if (CA_DLL_GetCycleData) {
        unsigned char* raw_buffer = nullptr;
        const char* error_cstr = nullptr;
        unsigned long buffer_length_in_bytes;
        WriteDebug("GCD")
        bool result = CA_DLL_GetCycleData(&raw_buffer, &buffer_length_in_bytes, &CycleNumber,
                                          reinterpret_cast<unsigned long*>(&LastCycleEndTime),
                                          reinterpret_cast<unsigned long*>(&LastCycleStartPreTriggerTime),
                                          &CycleError, &error_cstr);
        WriteDebug("x\n")
        buffer = reinterpret_cast<unsigned int*>(raw_buffer);
        if (error_cstr) ErrorMessages = QString::fromUtf8(error_cstr);
        else ErrorMessages.clear();
        buffer_length = buffer_length_in_bytes/4;
        return result;
    }
    return false;
}

bool CControlAPI::ClearAnalogInputQueue() {
    if (CA_DLL_ClearAnalogInputQueue) {
        WriteDebug("CAIQ")
        bool ret = CA_DLL_ClearAnalogInputQueue();
        WriteDebug("x ")
    }
    return false;
}

bool CControlAPI::HasInterlockTriggered() {
    if (CA_DLL_HasInterlockTriggered) {
        WriteDebug("HIT")
        bool ret = CA_DLL_HasInterlockTriggered();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::ResetInterlock() {
    if (CA_DLL_ResetInterlock) {
        WriteDebug("RI")
        bool ret = CA_DLL_ResetInterlock();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::SetExternalTrigger(bool ExternalTrigger0, bool ExternalTrigger1) {
    WriteDebug("SET")
    if (CA_DLL_SetExternalTrigger)
        CA_DLL_SetExternalTrigger(ExternalTrigger0, ExternalTrigger1);
    WriteDebug("x ")
}

void CControlAPI::SetPeriodicTrigger(double PeriodicTriggerPeriod_in_ms, double PeriodicTriggerAllowedWaitTime_in_ms) {
    WriteDebug("SPT")
    if (CA_DLL_SetPeriodicTrigger)
        CA_DLL_SetPeriodicTrigger(PeriodicTriggerPeriod_in_ms, PeriodicTriggerAllowedWaitTime_in_ms);
    WriteDebug("x ")
}

bool CControlAPI::GetPeriodicTriggerError() {
    if (CA_DLL_GetPeriodicTriggerError) {
        WriteDebug("GPTE")
        bool ret = CA_DLL_GetPeriodicTriggerError();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::SetExternalClock(bool ExternalClock0, bool ExternalClock1) {
    WriteDebug("SEC")
    if (CA_DLL_SetExternalClock)
        CA_DLL_SetExternalClock(ExternalClock0, ExternalClock1);
    WriteDebug("x ")
}

bool CControlAPI::ResetFPGA() {
    if (CA_DLL_ResetFPGA) {
        WriteDebug("RF")
        bool ret = CA_DLL_ResetFPGA();
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::SetupSerialPort(unsigned char port_number, unsigned long baud_rate) {
    if (CA_DLL_SetupSerialPort) {
        WriteDebug("SSP")
        bool ret = CA_DLL_SetupSerialPort(port_number, baud_rate);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

void CControlAPI::Ramp(unsigned char* output_name, double start_value, double end_value, double ramp_time_in_ms, double timestep_in_ms) {
    WriteDebug("R")
    if (CA_DLL_Ramp)
        CA_DLL_Ramp(reinterpret_cast<const char*>(output_name), start_value, end_value, ramp_time_in_ms, timestep_in_ms);
    WriteDebug("x ")
}

void CControlAPI::Wait(double time_in_ms, unsigned long ID) {
    WriteDebug("W")
    if (CA_DLL_Wait)
        CA_DLL_Wait(time_in_ms, ID);
    WriteDebug("x ")
}

void CControlAPI::WaitTillBusBufferEmpty(unsigned long ID) {
    WriteDebug("WTBBE")
    if (CA_DLL_WaitTillBusBufferEmpty)
        CA_DLL_WaitTillBusBufferEmpty(ID);
    WriteDebug("x ")
}

void CControlAPI::WaitTillRampsEnd(unsigned long ID) {
    WriteDebug("WTRE")
    if (CA_DLL_WaitTillRampsEnd)
        CA_DLL_WaitTillRampsEnd(ID);
    WriteDebug("x ")
}

void CControlAPI::StopRamps() {
    WriteDebug("SR")
    if (CA_DLL_StopRamps)
        CA_DLL_StopRamps();
    WriteDebug("x ")
}

void CControlAPI::SwitchDebugMode(bool OnOff, bool DebugTimingOnOff) {
    // If this is supported by the DLL, add pointer and call here
}

bool CControlAPI::WriteToSerial(unsigned int port_nr, unsigned char* command, unsigned long length) {
    if (CA_DLL_WriteToSerial) {
        WriteDebug("WTS")
        bool ret = CA_DLL_WriteToSerial(port_nr, reinterpret_cast<const char*>(command), length);
        WriteDebug("x ")
    }
    return false;
}

bool CControlAPI::WriteToI2C(unsigned int port_nr, unsigned char* command, unsigned long length) {
    if (CA_DLL_WriteToI2C) {
        WriteDebug("WTI2C")
        bool ret = CA_DLL_WriteToI2C(port_nr, reinterpret_cast<const char*>(command), length);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::WriteToSPI(unsigned int port_nr, unsigned char* command, unsigned long length) {
    if (CA_DLL_WriteToSPI) {
        WriteDebug("WTSPI")
        bool ret = CA_DLL_WriteToSPI(port_nr, reinterpret_cast<const char*>(command), length);
        WriteDebug("x ")
        return ret;
    }
    return false;
}

bool CControlAPI::GetSequenceDuration(double &SequenceDuration_in_ms) {
    if (CA_DLL_GetSequenceDuration) {
        WriteDebug("GSD")
        SequenceDuration_in_ms = CA_DLL_GetSequenceDuration();
        WriteDebug("x ")
        return true;
    }
    return false;
}


#else

bool CControlAPI::ConnectToLowLevelSoftware(QTelnet * atelnet, const char* ParamFileName, const char* IP,  const bool Debug, const QString DebugFileDirectory, unsigned long port, double timeout_in_seconds) {
    if (atelnet == NULL) return false;
    telnet = atelnet;
    if( telnet->isConnected() )
        telnet->disconnectFromHost();
    telnet->connectToHost(IP, port);
    ConnectedToLowLevelSoftware = true;
    return true;
}

void CControlAPI::DisconnectFromLowLevelSoftware() {

}

void CControlAPI::ConfigureControlAPI(bool DisplayCommandErrors) {
    telnet->writeString("ConfigureControlAPI");
    telnet->writeBool(DisplayCommandErrors);
}

QString CControlAPI::GetError() {
    telnet->writeString("GetError");
    QString error;
    if (telnet->readString(error, telnet->standard_timeout_in_seconds)) {
        return error;
    }
    return QString();
}

void CControlAPI::StoreSequenceInMemory(bool DoStoreSequenceInMemory) {
    telnet->writeString("StoreSequenceInMemory");
    telnet->writeBool(DoStoreSequenceInMemory);
}

void CControlAPI::SwitchDebugMode(bool OnOff, bool DebugTimingOnOff) {
    telnet->writeString("SwitchDebugMode");
    telnet->writeBool(OnOff);
    telnet->writeBool(DebugTimingOnOff);
}

bool CControlAPI::ConnectToSequencer(const char* IP, unsigned long port, double timeout_in_seconds) {ErrorNotYetImplemented(); return false;} //automatically executed during program start; needs to be only called if FPGA has been reset


bool CControlAPI::CheckIfSequencerReady(double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptCheckIfSequencerReady(b, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptCheckIfSequencerReady(bool &b, double timeout_in_seconds) {
    telnet->writeString("CheckIfSequencerReady");
    telnet->writeDouble(timeout_in_seconds);
    return telnet->readBool(b, timeout_in_seconds + telnet->standard_timeout_in_seconds);
}


bool CControlAPI::CheckIfLowLevelSoftwareReady(double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptCheckIfLowLevelSoftwareReady(b, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptCheckIfLowLevelSoftwareReady(bool &b, double timeout_in_seconds) {
    telnet->writeString("CheckIfLowLevelSoftwareReady");
    return telnet->readBool(b, timeout_in_seconds + telnet->standard_timeout_in_seconds);
}

long CControlAPI::Command(const char* command) {
    telnet->writeString("Command");
    telnet->writeString(command);
    return 0;
    //return telnet->readLong(); //0 if everything fine; number of the first command that went wrong, if something went wrong
} // only used by ethernet interface; in C, just call the commands declared in IOList.h


long CControlAPI::GetLastCommandLineNumber() {
    unsigned int attempts = 0;
    long success = -1;
    long l = -1;
    while ((attempts<10) && (!(success = AttemptGetLastCommandLineNumber(l)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return -1;
    return l;
}

long CControlAPI::AttemptGetLastCommandLineNumber(long &l) {
    telnet->writeString("GetLastCommandLineNumber");
    return telnet->readLong(l);
}

bool CControlAPI::DidCommandErrorOccur(long& ErrorLineNr, QString& CodeWithError, double timeout_in_seconds) { //returns true if error occured
    unsigned int attempts = 0;
    bool success = false;
    while ((attempts<10) && (!(success = AttemptDidCommandErrorOccur(ErrorLineNr, CodeWithError, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return true;
    return (ErrorLineNr != -1 );
}

bool CControlAPI::AttemptDidCommandErrorOccur(long& ErrorLineNr, QString& CodeWithError, double timeout_in_seconds) {
    telnet->writeString("DidCommandErrorOccur");
    CodeWithError = "";
    ErrorLineNr = 0;
    if (!telnet->readLong(ErrorLineNr, timeout_in_seconds + telnet->standard_timeout_in_seconds )) return false;
    if (!telnet->readString(CodeWithError, timeout_in_seconds + telnet->standard_timeout_in_seconds)) return false;
    return true;
}

void CControlAPI::ProgramSequence() {
    telnet->writeString("ProgramSequence");
}

void CControlAPI::SwitchToDirectOutputMode() {
    telnet->writeString("SwitchToDirectOutputMode");
}



bool CControlAPI::StartSequence(bool ShowRunProgressDialog, double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptStartSequence(b, ShowRunProgressDialog, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptStartSequence(bool &b, bool ShowRunProgressDialog, double timeout_in_seconds) {
    telnet->writeString("StartSequence");
    telnet->writeBool(ShowRunProgressDialog);
    return telnet->readBool(b, timeout_in_seconds);
}

bool CControlAPI::IsSequenceRunning() {
    unsigned int attempts = 0;
    bool success = false;
    bool running;
    while ((attempts<10) && (!(success = AttemptIsSequenceRunning(running)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return running;
}

bool CControlAPI::AttemptIsSequenceRunning(bool &running) {
    return telnet->readBool(running);
}


bool CControlAPI::WaitTillSequenceEnds(double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptWaitTillSequenceEnds(b, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptWaitTillSequenceEnds(bool &b, double timeout_in_seconds) {
    telnet->writeString("WaitTillSequenceEnds");
    telnet->writeDouble(timeout_in_seconds);
    return telnet->readBool(b, timeout_in_seconds + telnet->standard_timeout_in_seconds);
}

bool CControlAPI::IsCycling(double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptIsCycling(b, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptIsCycling(bool &b, double timeout_in_seconds) {
    telnet->writeString("IsCycling");
    return telnet->readBool(b, timeout_in_seconds + telnet->standard_timeout_in_seconds);
}


bool CControlAPI::DataAvailable(double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptDataAvailable(b, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptDataAvailable(bool &b, double timeout_in_seconds) {
    telnet->writeString("DataAvailable");
    return telnet->readBool(b, timeout_in_seconds + telnet->standard_timeout_in_seconds);
}

double CControlAPI::GetTime_in_ms() {
    unsigned int attempts = 0;
    bool success = false;
    double d;
    while ((attempts<10) && (!(success = DataAvailable(d)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return -1;
    return d;
}

bool CControlAPI::AttemptGetTime_in_ms(double &d) {
    telnet->writeString("GetTime_in_ms");
    return telnet->readDouble(d);
}

void CControlAPI::GoBackInTime(double aTimeJump_in_ms, unsigned int ID) {
    char command[100];
    sprintf(command, "GoBackInTime(%f, %d)", aTimeJump_in_ms, ID);
    Command(command);
}

void CControlAPI::GoToTime(double aTime_in_ms, unsigned int ID) {
    char command[100];
    sprintf(command, "GoToTime(%f, %d)", aTime_in_ms, ID);
    Command(command);
}

void CControlAPI::ReturnToCurrentTime(unsigned int ID) {
    char command[100];
    sprintf(command, "ReturnToCurrentTime(%d)", ID);
    Command(command);
}

void CControlAPI::FinishLastGoBackInTime(unsigned int ID) {
    char command[100];
    sprintf(command, "FinishLastGoBackInTime(%d)", ID);
    Command(command);
}

void CControlAPI::Ramp(unsigned char* output_name, double start_value /* use LastValue for last value */, double end_value, double ramp_time_in_ms, double timestep_in_ms) {
    char command[100];
    sprintf(command, "Ramp(\"%s\", %f, %f, %f, %f)", output_name, start_value, end_value, ramp_time_in_ms, timestep_in_ms);
    Command(command);
}

void CControlAPI::Wait(double time_in_ms, unsigned long ID) {
    char command[100];
    sprintf(command, "Wait(%f, %d)", time_in_ms, ID);
    Command(command);
}

void CControlAPI::WaitTillBusBufferEmpty(unsigned long ID) {
    char command[100];
    sprintf(command, "WaitTillBusBufferEmpty(%d)", ID);
    Command(command);
}

void CControlAPI::WaitTillRampsEnd(unsigned long ID) {
    char command[100];
    sprintf(command, "WaitTillRampsEnd(%d)", ID);
    Command(command);
}

void CControlAPI::StopRamps() {
    Command("StopRamps");
}



bool CControlAPI::StartCycling(long readout_pre_trigger_in_ms, long soft_pre_trigger_in_ms, bool TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, bool display_progress_dialog) {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptStartCycling(b, readout_pre_trigger_in_ms, soft_pre_trigger_in_ms, TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, display_progress_dialog)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}


bool CControlAPI::AttemptStartCycling(bool &b, long readout_pre_trigger_in_ms, long soft_pre_trigger_in_ms, bool TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, bool display_progress_dialog) {
    telnet->writeString("StartCycling");
    telnet->writeLong(readout_pre_trigger_in_ms);
    telnet->writeLong(soft_pre_trigger_in_ms);
    telnet->writeBool(TransmitOnlyDifferenceBetweenCommandSequenceIfPossible);
    telnet->writeBool(display_progress_dialog);
    return telnet->readBool(b, telnet->standard_timeout_in_seconds);
}

void CControlAPI::StopCycling() {
    telnet->writeString("StopCycling");
}

void CControlAPI::ResetCycleNumber() {
    telnet->writeString("ResetCycleNumber");
}


bool CControlAPI::GetNextCycleStartTimeAndNumber(long &TimeTillNextCycleStart_in_ms, long & NextCycleNumber, double timeout_in_seconds) {
    unsigned int attempts = 0;
    bool success = false;
    while ((attempts<10) && (!(success = AttemptGetNextCycleStartTimeAndNumber(TimeTillNextCycleStart_in_ms, NextCycleNumber, timeout_in_seconds)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    return success;
}

bool CControlAPI::AttemptGetNextCycleStartTimeAndNumber(long & TimeTillNextCycleStart_in_ms, long & NextCycleNumber, double timeout_in_seconds) {
    telnet->writeString("GetNextCycleStartTimeAndNumber");
    bool ok = telnet->readLong(TimeTillNextCycleStart_in_ms, timeout_in_seconds + telnet->standard_timeout_in_seconds);
    if (ok) ok = telnet->readLong(NextCycleNumber, timeout_in_seconds + telnet->standard_timeout_in_seconds);
    return ok;
}


bool CControlAPI::GetSequenceDuration(double &SequenceDuration_in_ms) {
    unsigned int attempts = 0;
    bool success = false;
    while ((attempts<10) && (!(success = AttemptGetSequenceDuration( SequenceDuration_in_ms )))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    return success;
}

bool CControlAPI::AttemptGetSequenceDuration(double &SequenceDuration_in_ms) {
    telnet->writeString("GetSequenceDuration");
    return telnet->readDouble(SequenceDuration_in_ms);
}

void CControlAPI::ReplaceCommand(unsigned long cycle_number, unsigned int command_line_nr, const char* new_command) {
    telnet->writeString("ReplaceCommand");
    telnet->writeLong(cycle_number);
    telnet->writeLong(command_line_nr);
    telnet->writeString(new_command);
}


// not implemented in V0.1
bool CControlAPI::ProgramInterlockSequence() {ErrorNotYetImplemented(); return false;}

bool CControlAPI::InterruptSequence() {ErrorNotYetImplemented(); return false;}

void CControlAPI::StartAnalogInAcquisition(unsigned char sequencer, unsigned char SPI_port, unsigned char SPI_CS, unsigned int channel_number, unsigned int number_of_datapoints, double delay_between_datapoints_in_ms) {
    char command[100];
    unsigned int _Sequencer = sequencer;
    unsigned int _SPI_port = SPI_port;
    unsigned int _SPI_CS = SPI_CS;
    sprintf(command, "StartAnalogInAcquisition(%d, %d, %d, %d, %d, %f)", _Sequencer, _SPI_port, _SPI_CS, channel_number, number_of_datapoints, delay_between_datapoints_in_ms);
    Command(command);
}




bool CControlAPI::WaitTillEndOfSequenceThenGetInputData(unsigned int*& buffer, unsigned long& buffer_length, double timeout_in_seconds) {//}, bool format32bit) {
    unsigned int attempts = 0;
    bool success = false;
    buffer = nullptr;
    while ((attempts<10) && (!(success = AttemptWaitTillEndOfSequenceThenGetInputData(buffer,  buffer_length, timeout_in_seconds) ))) {
        if (buffer!=nullptr) delete[] buffer;
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    return success;
}


bool CControlAPI::AttemptWaitTillEndOfSequenceThenGetInputData(unsigned int*& buffer, unsigned long& buffer_length, double timeout_in_seconds) {//}, bool format32bit) {
    buffer = NULL;
    buffer_length = 0;
    //WaitTillSequenceEnds(timeout_in_seconds); //ToDo: check: This should not be needed
    telnet->writeString("WaitTillEndOfSequenceThenGetInputData");
    telnet->writeDouble(timeout_in_seconds);
    bool success;
    if (telnet->readBool(success, timeout_in_seconds + telnet->standard_timeout_in_seconds)) {
        if (success) {
            long buffer_length_in_bytes;
            if (!telnet->readLong(buffer_length_in_bytes)) {
                //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : no length returned");
                return false;
            }
            buffer_length = buffer_length_in_bytes/4;
            if ((buffer_length_in_bytes<0) || (buffer_length_in_bytes>1000000)) {
                //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : wrong buffer_length; if too long, increase allowed length in code");
                return false;
            }
            //unsigned long* buffer_long = new unsigned long[buffer_length];
            buffer = new unsigned int[buffer_length];
            if (!telnet->readBuffer((unsigned char*)buffer, buffer_length_in_bytes, timeout_in_seconds)) {
                //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : no data returned, expected " + QString::number(buffer_length_in_bytes) +  "bytes");
                return false;
            }
            else {
                //ToDo (highly optional): enable extended 32-bit data readout
                //for (long n = 0; n<buffer_length; n++) ((unsigned int*)buffer)[n] = (unsigned int) (buffer_long[n] & 0x0000FFFF);
                //delete[] buffer_long;
                return true;
            }
        } else {
            //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : no success");
            return false;
        }
    } else {
        //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : no OK returned");
        return false;
    }
}


//This is a cheap way to make internet connection more reliable. ToDo: make more elegant.
bool CControlAPI::GetCycleData(unsigned int*& buffer, unsigned long& buffer_length,  long &CycleNumber, long &LastCycleEndTime, long &LastCycleStartPreTriggerTime, bool &CycleError, QString &ErrorMessages, double timeout_in_seconds) {
    if (buffer) delete[] buffer;
    buffer = NULL;
    unsigned int attempts = 0;
    bool success = false;
    while ((attempts<10) && (!(success = AttemptGetCycleData(buffer, buffer_length,  CycleNumber, LastCycleEndTime, LastCycleStartPreTriggerTime, CycleError, ErrorMessages, timeout_in_seconds)))) {
        if (buffer) delete[] buffer;
        buffer = NULL;
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    return success;
}


bool CControlAPI::AttemptGetCycleData(unsigned int*& buffer, unsigned long& buffer_length,  long &CycleNumber, long &LastCycleEndTime, long &LastCycleStartPreTriggerTime, bool &CycleError, QString &ErrorMessages, double timeout_in_seconds) {//, bool format32bit) {
    buffer = NULL;
    buffer_length = 0;
    CycleNumber = -1;
    CycleError = true;
    LastCycleEndTime = 0;
    LastCycleStartPreTriggerTime = 0;
    telnet->writeString("GetCycleData");
    bool success;
    if (telnet->readBool(success, timeout_in_seconds +  telnet->standard_timeout_in_seconds)) {
        if (success) {
            if (!telnet->readLong(CycleNumber)) {
                //MessageBox("CControlAPI::GetCycleData : no CycleNumber returned");
                return false;
            }
            if (!telnet->readLong(LastCycleEndTime)) {
                //MessageBox("CControlAPI::GetCycleData : no LastCycleEndTime returned");
                return false;
            }
            if (!telnet->readLong(LastCycleStartPreTriggerTime)) {
                //MessageBox("CControlAPI::GetCycleData : no LastCycleStartPreTriggerTime returned");
                return false;
            }
            if (!telnet->readBool(CycleError)) {
                //MessageBox("CControlAPI::GetCycleData : no CycleError returned");
                return false;
            }
            if (!telnet->readString(ErrorMessages)) {
                //MessageBox("CControlAPI::GetCycleData : no ErrorMessages returned");
                return false;
            }
            long buffer_length_in_bytes;
            if (!telnet->readLong(buffer_length_in_bytes)) {
                //MessageBox("CControlAPI::GetCycleData : no length returned");
                return false;
            }
            buffer_length = buffer_length_in_bytes/4;
            if ((buffer_length_in_bytes<0) || (buffer_length_in_bytes>1000000)) {
                //MessageBox("CControlAPI::GetCycleData : wrong buffer_length; if too long, increase allowed length in code");
                return false;
            }
            //unsigned long* buffer_long = new unsigned long[buffer_length];
            buffer = new unsigned int[buffer_length];
            if (!telnet->readBuffer((unsigned char*)buffer, buffer_length_in_bytes, timeout_in_seconds)) {
                //MessageBox("CControlAPI::WaitTillEndOfSequenceThenGetInputData : no data returned, expected " + QString::number(buffer_length_in_bytes) +  "bytes");
                return false;
            }
            else {
                //ToDo (highly optional): enable extended 32-bit data readout
                //for (long n = 0; n<buffer_length; n++) ((unsigned int*)buffer)[n] = (unsigned int) (buffer_long[n] & 0x0000FFFF);
                //delete[] buffer_long;
                return true;
            }
        } else {
            //MessageBox("CControlAPI::GetCycleData : no success");
            return false;
        }
    } else {
        //MessageBox("CControlAPI::GetCycleData : no OK returned");
        return false;
    }
}


bool CControlAPI::ClearAnalogInputQueue() {ErrorNotYetImplemented(); return false;}
bool CControlAPI::HasInterlockTriggered() {ErrorNotYetImplemented(); return false;}
bool CControlAPI::ResetInterlock() {ErrorNotYetImplemented(); return false;}

void CControlAPI::SetPeriodicTrigger(double PeriodicTriggerPeriod_in_ms, double PeriodicTriggerAllowedWaitTime_in_ms) {
    telnet->writeString("SetPeriodicTrigger");
    telnet->writeDouble(PeriodicTriggerPeriod_in_ms);
    telnet->writeDouble(PeriodicTriggerAllowedWaitTime_in_ms);
}



bool CControlAPI::GetPeriodicTriggerError() {
    unsigned int attempts = 0;
    bool success = false;
    bool b;
    while ((attempts<10) && (!(success = AttemptGetPeriodicTriggerError(b)))) {
        telnet->disconnectFromRemote();
        Sleep_ms(1000);
        attempts++;
    }
    if (attempts>=10) return false;
    return b;
}

bool CControlAPI::AttemptGetPeriodicTriggerError(bool &b) {
    telnet->writeString("GetPeriodicTriggerError");
    return telnet->readBool(b);
}

void CControlAPI::SetExternalTrigger(bool ExternalTrigger0, bool ExternalTrigger1) {
    telnet->writeString("SetExternalTrigger");
    telnet->writeBool(ExternalTrigger0);
    telnet->writeBool(ExternalTrigger1);
}

void CControlAPI::SetExternalClock(bool ExternalClock0, bool ExternalClock1 ) {
    telnet->writeString("SetExternalClock");
    telnet->writeBool(ExternalClock0);
    telnet->writeBool(ExternalClock1);
}

bool CControlAPI::ResetFPGA() {
    telnet->writeString("ResetFPGA");
}


// not implemented in V0.1
bool CControlAPI::SetupSerialPort(unsigned char port_number, unsigned long baud_rate) { ErrorNotYetImplemented(); return false; }
bool CControlAPI::WriteToSerial(unsigned int port_nr, unsigned char* command, unsigned long length) {ErrorNotYetImplemented(); return false;}
bool CControlAPI::WriteToI2C(unsigned int port_nr, unsigned char* command, unsigned long length) {ErrorNotYetImplemented(); return false;}
bool CControlAPI::WriteToSPI(unsigned int port_nr, unsigned char* command, unsigned long length) {ErrorNotYetImplemented(); return false;}


#endif

bool CControlAPI::WaitForDirectOutputModeCommandToFinish(double timeout_in_seconds) {
    bool ok = CheckIfLowLevelSoftwareReady(timeout_in_seconds);
    if (!ok) MessageBox("CControlAPI::WaitForDirectOutputModeCommandToFinish : command not finished within given timeout of "+ QString::number(timeout_in_seconds)+"s");
    return ok;
}
