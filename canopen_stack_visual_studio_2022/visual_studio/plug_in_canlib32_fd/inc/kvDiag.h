/*
 *                   Copyright 2002-2019 by KVASER AB, SWEDEN
 *
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 *
 */

/**
 *
 * \file kvDiag.h
 * \brief The CANtegrity API is based on high frequency sampling of the CAN bus.
 * This allows it to be used for various purposes, such as calculating the bitrate
 * on the bus and measuring clock speeds of devices on the bus.
 * \note \parblock Not supported in linux! \endparblock
 * \note \parblock The CANtegrity API is preliminary and will change! \endparblock
 *
 */

#ifndef _KVDIAG_H_
#define _KVDIAG_H_

#include <canlib.h>
#include <stdint.h>

/** \ingroup kvdiag_diagnostics */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name DIAG_ANALYZER_TYPE_xxx
 * Analyzer type.
 * \anchor DIAG_ANALYZER_TYPE_xxx
 * @{
 */
#define DIAG_ANALYZER_TYPE_DEFAULT 0 /**< Default. */
/** @} */


/**
 * \name DIAG_PROGRAM_TYPE
 * Possible programs that can be run on an analyzer.
 * Used with \ref kvDiagSetProgram().
 * @{
 */
#define DIAG_PROGRAM_TYPE_NORMAL   0
#define DIAG_PROGRAM_TYPE_AUTOBAUD 1
/** @} */

/// @cond INTERNAL_USE
#define DIAG_PROGRAM_TYPE_HALT     2
#define DIAG_PROGRAM_TYPE_GENERIC  3

#define DIAG_PROGRAM_STATE_UNKNOWN      0
#define DIAG_PROGRAM_STATE_RUNNING      1
#define DIAG_PROGRAM_STATE_STOPPED      2
/// @endcond


/**
* Output when using \ref kvDiagCalculateBitrate(), housed in \ref bitrates_t.
*/
typedef struct {
  double bitrate;   ///< Bitrate in kBit/s
  int quality;      ///< Quality value in % (0..100)
} bitrate_t;


/**
* Output when using \ref kvDiagCalculateBitrate().
*/
typedef struct {
  bitrate_t bitrate;      ///< Bitrate, arbitration bitrate if using FD with BRS
  bitrate_t bitrate_brs;  ///< Data bitrate if using FD with BRS
} bitrates_t;

/**
 * Output when using \ref kvDiagReadSample() and \ref kvDiagReadSampleWait().
 */
typedef struct {
  struct {
    int version;        ///< Struct version
    int seqno;          ///< Sequence number
    int type;           ///< Analyzer type
  } header;
  struct {
    int id;             ///< CAN id
    int64_t time;       ///< CPU ticks since device boot
    char data[64];      ///< CAN data
    int flag;           ///< CAN flags, \ref canMSG_STD, \ref canMSG_EXT, \ref canMSG_RTR
    int dlc;            ///< CAN DLC
  } msg;
  struct {
    int64_t startTime;    ///< Time of the first edge (start bit)
    int sampleFreq;       ///< Clock frequency of the analyzer
    int startValue;       ///< Value prior to the first edge (start bit)
    int edgeCount;        ///< Number of edges in \a edgeTimes
    int edgeTimes[8192];  ///< Time since previous edge in analyzer ticks at \a sampleFreq
  } sample;
} kvDiagSample;

typedef struct {
  int analyzerNo;
  int type;
  int version_major;
  int version_minor;
} kvAnalyzerInfo_t;


/**
 * \ingroup kvdiag_diagnostics
 * Get the number of analyzers.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel.
 * \param[out] analyzerCount Number of analyzers.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagGetNumberOfAnalyzers()
 */
canStatus CANLIBAPI kvDiagGetNumberOfAnalyzers(const canHandle canHnd,
                                               int *analyzerCount);

/**
 * \ingroup kvdiag_diagnostics
 * Get analyzer info.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel.
 * \param[inout] info for analyzer info.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagGetAnalyzerInfo()
 */
canStatus CANLIBAPI kvDiagGetAnalyzerInfo(const canHandle canHnd,
                                          kvAnalyzerInfo_t* info);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Attach an analyzer to the a CAN channel handle. The device needs to
 * have the CANtegrity capability. See \ref page_user_guide_kvdiag for how
 * to check this.
 *
 * \note A CAN channel can currently only have one attached Analyzer.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel of a KSI capable device.
 * \param[in] type DIAG_ANALYZER_TYPE_XXX.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagDetachAnalyzer()
 */
canStatus CANLIBAPI kvDiagAttachAnalyzer(const canHandle canHnd, int type);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Detach the analyzer from the CAN channel handle.
 *
 * \note \parblock Currently only one analyzer is supported. \endparblock
 *
 * \note \parblock The kvDiag API is preliminary and will change! \endparblock
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 */
canStatus CANLIBAPI kvDiagDetachAnalyzer(const canHandle canHnd);

/**
 * \ingroup kvdiag_diagnostics
 * Choose analyzer program to run.
 *
 * \p configBuffer is a JSON string containing configuration parameters.
 * The following is an example configuration yielding a bitrate of 1MBit/s:
 * \code{.c}
 *    const char *configBuffer = "{\"tseg1\" : 59, \"tseg2\" : 20, \"sjw\" : 16, \"brp\" : 1}";
 * \endcode
 *    where
 * \code{.unparsed}
 *     tseg1     Time segment 1, that is, the number of quanta from (but not
 *               including) the Sync Segment to the sampling point.
 *
 *     tseg2     Time segment 2, that is, the number of quanta from the sampling
 *               point to the end of the bit.
 *
 *     sjw       The Synchronization Jump Width.
 *
 *     brp       Bitrate prescaler.
 * \endcode
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analayzer.
 * \param[in] programType Analyzer program, either \ref DIAG_PROGRAM_TYPE_NORMAL or
 *                        \ref DIAG_PROGRAM_TYPE_AUTOBAUD.
 * \param[in] configBuffer A configuration string in JSON format specifying bus parameters.
 *
 * \return \ref canOK (zero) if success.
 * \return \ref canERR_xxx (negative) if failure.
 */
canStatus CANLIBAPI kvDiagSetProgram(const canHandle canHnd,
                                     int programType,
                                     const char *const configBuffer);

/**
 * \ingroup kvdiag_diagnostics
 * Start data acquisition.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagStop()
 */
canStatus CANLIBAPI kvDiagStart(const canHandle canHnd);

/**
 * \ingroup kvdiag_diagnostics
 * Stop data acquisition.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 * \sa \ref kvDiagSetProgram()
 * \sa \ref kvDiagStart()
 */
canStatus CANLIBAPI kvDiagStop(const canHandle canHnd);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Run with \ref DIAG_PROGRAM_TYPE_NORMAL, see \ref page_user_guide_kvdiag.
 *
 * Read (and consume) one full sample from an analyzer.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 * \param[out] sample Pointer to struct to receive the sample.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 * \sa \ref kvDiagSetProgram()
 * \sa \ref kvDiagStart()
 * \sa \ref kvDiagReadSampleWait()
 */
canStatus CANLIBAPI kvDiagReadSample(const canHandle canHnd,
                                     kvDiagSample *sample);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Run with \ref DIAG_PROGRAM_TYPE_NORMAL, see \ref page_user_guide_kvdiag.
 *
 * Read (and consume) one full sample from an analyzer. Will wait until a message
 * arrives or a timeout occurs.It can take up to 100 ms for this function to
 * return after a new sample is available.
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 * \param[out] sample Pointer to struct to receive the sample.
 * \param[in] timeout Number of milliseconds to wait for the next sample.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_NOMSG (negative) if there was no message available.
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 * \sa \ref kvDiagSetProgram()
 * \sa \ref kvDiagStart()
 * \sa \ref kvDiagReadSample()
 */
canStatus CANLIBAPI kvDiagReadSampleWait(const canHandle canHnd,
                                         kvDiagSample *sample,
                                         int timeout);


/**
 * \ingroup kvdiag_diagnostics
 *
 * Run with \ref DIAG_PROGRAM_TYPE_AUTOBAUD, see \ref page_user_guide_kvdiag.
 *
 * Calculate the bitrate on the CAN bus by sampling a series of CAN frames.
 * Uses all the sampled frames in the device buffer on each call.
 * Stores information between calls. May require several calls until the
 * quality value is sufficiently high.
 *
 * \note \parblock The Analyzer program \ref DIAG_PROGRAM_TYPE_AUTOBAUD will
 * stop processing after 100 frames have been received. To restart processing,
 * call \ref kvDiagResetBitrateCalculation(). \endparblock
 *
 * \note \parblock For classic CAN \a btr.bitrate contains the calculated bitrate
 * and quality. Currently, FD is not supported so \a btr.bitrate_brs can be
 * disregarded. \endparblock
 *
 * \note \parblock The kvDiag API is preliminary and will change! \endparblock
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 * \param[out] btr Contains two sets of values for bitrate and quality.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 * \sa \ref kvDiagSetProgram()
 * \sa \ref kvDiagStart()
 * \sa \ref kvDiagResetBitrateCalculation()
*/
canStatus CANLIBAPI kvDiagCalculateBitrate(const canHandle canHnd, bitrates_t *btr);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Resets the bitrate calculation of \ref kvDiagCalculateBitrate().
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagCalculateBitrate()
*/
canStatus CANLIBAPI kvDiagResetBitrateCalculation(const canHandle canHnd);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Run with \ref DIAG_PROGRAM_TYPE_NORMAL, see \ref page_user_guide_kvdiag.
 *
 * Get the clock offset of a device relative to the clock of
 * the measuring device. This roughly equates to the
 * relative difference of the two frequencies.
 *
 * \code
 *  clockoffset = (T_id - T_diag) / T_diag = (f_diag - f_id) / f_id
 * \endcode

 * where
 * \code
 * T_diag     Time elapsed on the measuring device.
 *
 * T_id       Time elapsed on the target device.
 *
 * f_diag     Frequency of the measuring device.
 *
 * f_id       Frequency of the target device.
 * \endcode
 *
 * The result is expressed as a normal distribution over the
 * sampled frames used. The unit of measurement is ppt
 * (parts per trillion, 1e-12).
 *
 * The function stores information between calls. Call until
 * sufficient precision is reached.
 * Each call will consume one sampled frame from the device buffer.
 *
 * \note \parblock This function is in an alpha state! \endparblock
 *
 * \note \parblock The kvDiag API is preliminary and will change! \endparblock
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 * \param[in] id Id with which the device of interest is sending
 * \param[out] fMin Minimum drift encountered (parts per trillion)
 * \param[out] fMax Maximum drift encountered (parts per trillion)
 * \param[out] fMean Mean drift encountered (parts per trillion)
 * \param[out] fStd Standard deviation (parts per trillion)
 * \param[out] n Number of sampled frames used.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagAttachAnalyzer()
 * \sa \ref kvDiagSetProgram()
 * \sa \ref kvDiagStart()
 * \sa \ref kvDiagResetClockOffsetCalculation()
*/
canStatus CANLIBAPI kvDiagCalculateClockOffset(const canHandle canHnd,
                                               int id,
                                               double *fMin,
                                               double *fMax,
                                               double *fMean,
                                               double *fStd,
                                               int *n);

/**
 * \ingroup kvdiag_diagnostics
 *
 * Resets the clock offset calculation of \ref kvDiagCalculateClockOffset().
 *
 * \note The kvDiag API is preliminary and will change!
 *
 * \param[in] canHnd An open handle to a CAN channel with an attached analyzer.
 *
 * \return \ref canOK (zero) if success
 * \return \ref canERR_xxx (negative) if failure
 *
 * \sa \ref kvDiagCalculateClockOffset()
*/
canStatus CANLIBAPI kvDiagResetClockOffsetCalculation(const canHandle canHnd);


#ifdef __cplusplus
}
#endif

#endif // _KVDIAG_H_
