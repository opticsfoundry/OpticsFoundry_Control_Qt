#ifndef CONTROLAPI_H
#define CONTROLAPI_H

#include "QTCPIP.h"
#include <QLibrary>
#include <QFile>

#define USE_CA_DLL

#ifdef USE_CA_DLL



typedef void* HControlAPI;

/* Function pointer types for the following functions
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
API_EXPORT void ControlAPI_StartAnalogInAcquisition(unsigned int channel_number, unsigned int num_datapoints, double delay_ms);
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

typedef HControlAPI (*GetInstanceFunc)();
typedef bool (*CreateFunc)(const char*, bool, bool, bool);
typedef bool (*ConfigureFunc)(bool);
typedef void (*CleanupFunc)();
typedef long (*CommandFunc)(const char*);
typedef bool (*DidErrorFunc)(long*, const char**);
typedef const char* (*GetErrorFunc)();
typedef void (*StoreSequenceInMemoryFunc)(bool);
typedef void (*SwitchToDirectOutputModeFunc)();
typedef bool (*StartSequenceFunc)(bool);
typedef bool (*IsSequenceRunningFunc)();
typedef long (*GetLastCommandLineNumberFunc)();
typedef bool (*WaitTillSequenceEndsFunc)(double);
typedef bool  (*ResetFPGAFunc)();
typedef bool  (*ConnectToSequencerFunc)(const unsigned char*, unsigned long, double);
typedef bool  (*CheckIfSequencerReadyFunc)(double);
typedef void  (*ProgramSequenceFunc)();
typedef bool  (*ProgramInterlockSequenceFunc)();
typedef bool  (*ReplaceCommandFunc)(unsigned long, unsigned int, const char*);
typedef void  (*ReplaceCommandForNextCycleFunc)(unsigned int, const char*);
typedef void  (*ReplaceCommandsForNextCycleFunc)();
typedef void  (*ResetCommandListFunc)();
typedef bool  (*AssembleSequenceListFromMemoryFunc)();
typedef bool  (*StartCyclingFunc)(long, long, bool, bool);
typedef void  (*StopCyclingFunc)();
typedef bool  (*IsCyclingFunc)();
typedef bool  (*DataAvailableFunc)();
typedef bool  (*GetNextCycleStartTimeAndNumberFunc)(long*, long*);
typedef bool  (*ResetCycleNumberFunc)();
typedef bool  (*InterruptSequenceFunc)();
typedef void  (*WriteReadSPIFunc)(unsigned int, unsigned int, unsigned long long, unsigned long long, unsigned int);
typedef bool  (*WaitTillEndOfSequenceThenGetInputDataFunc)(unsigned char**, unsigned long*, unsigned long*, double);
typedef bool  (*GetCycleDataFunc)(unsigned char**, unsigned long*, long*, unsigned long*, unsigned long*, bool*, const char**);
typedef bool  (*ClearAnalogInputQueueFunc)();
typedef bool  (*HasInterlockTriggeredFunc)();
typedef bool  (*ResetInterlockFunc)();
typedef void  (*SetExternalTriggerFunc)(bool, bool);
typedef void  (*SetPeriodicTriggerFunc)(double, double);
typedef bool  (*GetPeriodicTriggerErrorFunc)();
typedef void  (*SetExternalClockFunc)(bool, bool);
typedef bool  (*SetupSerialPortFunc)(unsigned char, unsigned long);
typedef bool  (*WriteToSerialFunc)(unsigned int, const char*, unsigned long);
typedef bool  (*WriteToI2CFunc)(unsigned int, const char*, unsigned long);
typedef bool  (*WriteToSPIFunc)(unsigned int, const char*, unsigned long);
typedef double (*GetSequenceDurationFunc)();
typedef double (*GetTimeInMsFunc)();
typedef void  (*RampFunc)(const char*, double, double, double, double);
typedef void  (*WaitFunc)(double, unsigned long);
typedef void  (*WaitTillBusBufferEmptyFunc)(unsigned long);
typedef void  (*WaitTillRampsEndFunc)(unsigned long);
typedef void  (*StopRampsFunc)();
typedef void  (*DoNothingFunc)();
typedef void  (*StartAnalogInAcquisitionFunc)(unsigned char,unsigned char,unsigned char,unsigned int, unsigned int, double);
typedef void  (*StartXADCAnalogInAcquisitionFunc)(unsigned int, unsigned int, double);
typedef void  (*GoBackInTimeFunc)(double, unsigned int);
typedef void  (*GoToTimeFunc)(double, unsigned int);
typedef void  (*ReturnToCurrentTimeFunc)(unsigned int);
typedef void  (*FinishLastGoBackInTimeFunc)(unsigned int);
typedef void  (*WriteInputMemoryFunc)(unsigned long, bool, unsigned long);
typedef void  (*WriteSystemTimeToInputMemoryFunc)();
typedef void  (*SwitchDebugLEDFunc)(unsigned int);
typedef void  (*IgnoreTCPIPFunc)(bool);
typedef void  (*AddMarkerFunc)(unsigned char);

#endif




class CControlAPI
{
#ifdef USE_CA_DLL
private:
    QLibrary* CA_DLL_Lib;
    GetInstanceFunc CA_DLL_GetInstance;
    CreateFunc CA_DLL_Create;
    ConfigureFunc CA_DLL_Configure;
    CleanupFunc CA_DLL_Cleanup;
    CommandFunc CA_DLL_Command;
    DidErrorFunc CA_DLL_DidCommandErrorOccur;
    GetErrorFunc CA_DLL_GetLastError;
    StoreSequenceInMemoryFunc CA_DLL_StoreSequenceInMemory;
    SwitchToDirectOutputModeFunc CA_DLL_SwitchToDirectOutputMode;
    StartSequenceFunc CA_DLL_StartSequence;
    IsSequenceRunningFunc CA_DLL_IsSequenceRunning;
    GetLastCommandLineNumberFunc CA_DLL_GetLastCommandLineNumber;
    WaitTillSequenceEndsFunc CA_DLL_WaitTillSequenceEnds;
    ResetFPGAFunc CA_DLL_ResetFPGA;
    ConnectToSequencerFunc CA_DLL_ConnectToSequencer;
    CheckIfSequencerReadyFunc CA_DLL_CheckIfSequencerReady;
    ProgramSequenceFunc CA_DLL_ProgramSequence;
    ProgramInterlockSequenceFunc CA_DLL_ProgramInterlockSequence;
    ReplaceCommandFunc CA_DLL_ReplaceCommand;
    ReplaceCommandForNextCycleFunc CA_DLL_ReplaceCommandForNextCycle;
    ReplaceCommandsForNextCycleFunc CA_DLL_ReplaceCommandsForNextCycle;
    ResetCommandListFunc CA_DLL_ResetCommandList;
    AssembleSequenceListFromMemoryFunc CA_DLL_AssembleSequenceListFromMemory;
    StartCyclingFunc CA_DLL_StartCycling;
    StopCyclingFunc CA_DLL_StopCycling;
    IsCyclingFunc CA_DLL_IsCycling;
    DataAvailableFunc CA_DLL_DataAvailable;
    GetNextCycleStartTimeAndNumberFunc CA_DLL_GetNextCycleStartTimeAndNumber;
    ResetCycleNumberFunc CA_DLL_ResetCycleNumber;
    InterruptSequenceFunc CA_DLL_InterruptSequence;
    WriteReadSPIFunc CA_DLL_WriteReadSPI;
    WaitTillEndOfSequenceThenGetInputDataFunc CA_DLL_WaitTillEndOfSequenceThenGetInputData;
    GetCycleDataFunc CA_DLL_GetCycleData;
    ClearAnalogInputQueueFunc CA_DLL_ClearAnalogInputQueue;
    HasInterlockTriggeredFunc CA_DLL_HasInterlockTriggered;
    ResetInterlockFunc CA_DLL_ResetInterlock;
    SetExternalTriggerFunc CA_DLL_SetExternalTrigger;
    SetPeriodicTriggerFunc CA_DLL_SetPeriodicTrigger;
    GetPeriodicTriggerErrorFunc CA_DLL_GetPeriodicTriggerError;
    SetExternalClockFunc CA_DLL_SetExternalClock;
    SetupSerialPortFunc CA_DLL_SetupSerialPort;
    WriteToSerialFunc CA_DLL_WriteToSerial;
    WriteToI2CFunc CA_DLL_WriteToI2C;
    WriteToSPIFunc CA_DLL_WriteToSPI;
    GetSequenceDurationFunc CA_DLL_GetSequenceDuration;
    GetTimeInMsFunc CA_DLL_GetTimeInMs;
    RampFunc CA_DLL_Ramp;
    WaitFunc CA_DLL_Wait;
    WaitTillBusBufferEmptyFunc CA_DLL_WaitTillBusBufferEmpty;
    WaitTillRampsEndFunc CA_DLL_WaitTillRampsEnd;
    StopRampsFunc CA_DLL_StopRamps;
    DoNothingFunc CA_DLL_DoNothing;
    StartAnalogInAcquisitionFunc CA_DLL_StartAnalogInAcquisition;
    StartXADCAnalogInAcquisitionFunc CA_DLL_StartXADCAnalogInAcquisition;
    GoBackInTimeFunc CA_DLL_GoBackInTime;
    GoToTimeFunc CA_DLL_GoToTime;
    ReturnToCurrentTimeFunc CA_DLL_ReturnToCurrentTime;
    FinishLastGoBackInTimeFunc CA_DLL_FinishLastGoBackInTime;
    WriteInputMemoryFunc CA_DLL_WriteInputMemory;
    WriteSystemTimeToInputMemoryFunc CA_DLL_WriteSystemTimeToInputMemory;
    SwitchDebugLEDFunc CA_DLL_SwitchDebugLED;
    IgnoreTCPIPFunc CA_DLL_IgnoreTCPIP;
    AddMarkerFunc CA_DLL_AddMarker;
    HControlAPI CA_DLL_Handle;
    void Set_CA_DLL_CallsToNull();

#endif
public:
    QTelnet *telnet;

public:
    bool ConnectedToLowLevelSoftware;
    bool isConnected(bool no_error_message=true) const;
    unsigned int GetNumberReconnects() const;
    bool UsingDLL() const;
public:
    CControlAPI();
    virtual ~CControlAPI();
    void MessageBox(QString aMessage);
    bool ConnectToLowLevelSoftware(
        QTelnet *telnet, const char* ParamFileName, const char* IP="", const bool Debug = false, QString DebugFileDirectory = "", unsigned long port = 6342, double timeout_in_seconds = 1); //ok
    void DisconnectFromLowLevelSoftware();
    void ConfigureControlAPI(bool DisplayCommandErrors); //ok
    QString GetError();
    void StoreSequenceInMemory(bool DoStoreSequenceInMemory); //ok
    bool ConnectToSequencer(const char* IP, unsigned long port = 23, double timeout_in_seconds = 1); //ToDo
    bool CheckIfLowLevelSoftwareReady(double timeout_in_seconds = 2); //ok
    bool WaitForDirectOutputModeCommandToFinish(double timeout_in_seconds = 2);
    bool CheckIfSequencerReady(double timeout_in_seconds = 2); //ok
    long Command(const char* command); //ok
    bool DidCommandErrorOccur(long& ErrorLineNr, QString& CodeWithError, double timeout_in_seconds = 0); //ok
    long GetLastCommandLineNumber(); //ok
    void ProgramSequence(); //ok
    void SwitchToDirectOutputMode(); //ok
    bool ProgramInterlockSequence(); //next version
    void ReplaceCommand(unsigned long cycle_number, unsigned int command_line_nr, const char* new_command); //next version
    bool StartSequence(bool ShowRunProgressDialog = false, double timeout_in_seconds = 10); //ok
    void ResetCycleNumber();
    bool StartCycling(long readout_pre_trigger_in_ms = 100, long soft_pre_trigger_in_ms = 0, bool TransmitOnlyDifferenceBetweenCommandSequenceIfPossible = false, bool diplay_progress_dialog = false); //ok
    void StopCycling(); //ok
    bool IsCycling(double timeout_in_seconds = 0); //ok
    bool DataAvailable(double timeout_in_seconds = 0);
    bool GetNextCycleStartTimeAndNumber(long & TimeTillNextCycleStart_in_ms, long & NextCycleNumber, double timeout_in_seconds = 0); //ok
    bool IsSequenceRunning(); //ok
    bool WaitTillSequenceEnds(double timeout_in_seconds = 10); //ok
    bool InterruptSequence(); //next version
    double GetTime_in_ms(); //ok
    void GoBackInTime(double aTimeJump_in_ms, unsigned int ID = 1221); //ok, direct and inside Command()
    void GoToTime(double aTime_in_ms, unsigned int ID = 2112); //ok, direct and inside Command()
    void ReturnToCurrentTime(unsigned int ID = 2332); //ok, direct and inside Command()
    void FinishLastGoBackInTime(unsigned int ID = 3223); //ok, direct and inside Command()
    void StartAnalogInAcquisition(unsigned char sequencer, unsigned char SPI_port, unsigned char SPI_CS, unsigned int channel_number, unsigned int number_of_datapoints, double delay_between_datapoints_in_ms);
    bool WaitTillEndOfSequenceThenGetInputData(unsigned int*& buffer, unsigned long& buffer_length, double timeout_in_seconds = 10);//, bool format32bit = false);
    bool GetCycleData(unsigned int*& buffer, unsigned long& buffer_length,  long &CycleNumber, long &LastCycleEndTime, long &LastCycleStartPreTriggerTime, bool &CycleError, QString &ErrorMessages, double timeout_in_seconds = 0);//, bool format32bit = false);
    bool ClearAnalogInputQueue(); //next version
    bool HasInterlockTriggered(); //next version
    bool ResetInterlock(); //next version
    void SetExternalTrigger(bool ExternalTrigger0, bool ExternalTrigger1);
    void SetPeriodicTrigger(double PeriodicTriggerPeriod_in_ms, double PeriodicTriggerAllowedWaitTime_in_ms);
    bool GetPeriodicTriggerError();
    void SetExternalClock(bool ExternalClock0, bool ExternalClock1);
    bool ResetFPGA();
    bool SetupSerialPort(unsigned char port_number, unsigned long baud_rate);
    void Ramp(unsigned char* output_name, double start_value /* use LAST_VALUE for last value */, double end_value, double ramp_time_in_ms, double timestep_in_ms = 0.1); //ok, direct and inside Command()
    void Wait(double time_in_ms, unsigned long ID = 1234); //ok, direct and inside Command()
    void WaitTillBusBufferEmpty(unsigned long ID = 2345);
    void WaitTillRampsEnd(unsigned long ID = 4567);
    void StopRamps();
    void SwitchDebugMode(bool OnOff, bool DebugTimingOnOff);
    bool WriteToSerial(unsigned int port_nr, unsigned char* command, unsigned long length); //next version
    bool WriteToI2C(unsigned int port_nr, unsigned char* command, unsigned long length); //next version
    bool WriteToSPI(unsigned int port_nr, unsigned char* command, unsigned long length); //next version
    bool GetSequenceDuration(double &SequenceDuration_in_ms); //in ms

private:
    bool AttemptCheckIfLowLevelSoftwareReady(bool &b, double timeout_in_seconds=2);
    bool AttemptCheckIfSequencerReady(bool &b, double timeout_in_seconds = 2);
    bool AttemptDidCommandErrorOccur(long& ErrorLineNr, QString& CodeWithError, double timeout_in_seconds = 0);
    long AttemptGetLastCommandLineNumber(long &l);
    bool AttemptStartSequence(bool &b, bool ShowRunProgressDialog, double timeout_in_seconds);
    bool AttemptStartCycling(bool &b, long readout_pre_trigger_in_ms, long soft_pre_trigger_in_ms, bool TransmitOnlyDifferenceBetweenCommandSequenceIfPossible, bool display_progress_dialog);
    bool AttemptIsCycling(bool &b, double timeout_in_seconds=0);
    bool AttemptDataAvailable(bool &b, double timeout_in_seconds);
    bool AttemptGetNextCycleStartTimeAndNumber(long & TimeTillNextCycleStart_in_ms, long & NextCycleNumber, double timeout_in_seconds);
    bool AttemptIsSequenceRunning(bool &running);
    bool AttemptWaitTillSequenceEnds(bool &b, double timeout_in_seconds = 10);
    bool AttemptGetTime_in_ms(double &d);
    bool AttemptWaitTillEndOfSequenceThenGetInputData(unsigned int*& buffer, unsigned long& buffer_length, double timeout_in_seconds);
    bool AttemptGetCycleData(unsigned int*& buffer, unsigned long& buffer_length,  long &CycleNumber, long &LastCycleEndTime, long &LastCycleStartPreTriggerTime, bool &CycleError, QString &ErrorMessages, double timeout_in_seconds = 0);//, bool format32bit = false);
    bool AttemptGetPeriodicTriggerError(bool &b);
    bool AttemptGetSequenceDuration(double &SequenceDuration_in_ms);

private:
    QString DebugFileDirectory;
    QFile* DebugFile;
    QTextStream* DebugTextStream;
};

#endif // CONTROLAPI_H
