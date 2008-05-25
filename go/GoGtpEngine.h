//----------------------------------------------------------------------------
/** @file GoGtpEngine.h
    GtpEngine with GoBoard, GoPlayer and GoGame.
*/
//----------------------------------------------------------------------------

#ifndef GOGTPENGINE_H
#define GOGTPENGINE_H

#include "GoBoard.h"
#include "GoBook.h"
#include "GoGame.h"
#include "GoGtpTimeSettings.h"
#include "GoStaticLadder.h"
#include "GtpEngine.h"
#include "SgGtpCommands.h"
#include "SgPointArray.h"

//----------------------------------------------------------------------------

/** GtpEngine with GoBoard, GoPlayer and GoGame.
    @see gogtpenginecommands
*/
class GoGtpEngine
    : public GtpEngine
{
public:
    /** @page gogtpenginecommands GoGtpEngine Commands
        - @link CmdAllLegal() @c all_legal @endlink
        - @link CmdAllMoveValues() @c all_move_values @endlink
        - @link CmdBoardSize() @c boardsize @endlink
        - @link CmdClearBoard() @c clear_board @endlink
        - @link CmdFinalScore() @c final_score @endlink
        - @link CmdFixedHandicap() @c fixed_handicap @endlink
        - @link CmdGenMove() @c genmove @endlink
        - @link CmdGetKomi() @c get_komi @endlink
        - @link CmdGGUndo() @c gg-undo @endlink
        - @link CmdBoard() @c go_board @endlink
        - @link CmdCheckPerformance() @c go_check_performance @endlink
        - @link CmdClock() @c go_clock @endlink
        - @link CmdLadder() @c go_ladder @endlink
        - @link CmdParam() @c go_param @endlink
        - @link CmdParamRules() @c go_param_rules @endlink
        - @link CmdParamTimecontrol() @c go_param_timecontrol @endlink
        - @link CmdPlayerBoard() @c go_player_board @endlink
        - @link CmdPointNumbers() @c go_point_numbers @endlink
        - @link CmdPointInfo() @c go_point_info @endlink
        - @link CmdRules() @c go_rules @endlink
        - @link CmdSetInfo() @c go_set_info @endlink
        - @link CmdStaticLadder() @c go_static_ladder @endlink
        - @link CmdAnalyzeCommands() @c gogui-analyze_commands @endlink
        - @link CmdInterrupt() @c gogui-interrupt @endlink
        - @link CmdPlaySequence() @c gogui-play_sequence @endlink
        - @link CmdSetup() @c gogui-setup @endlink
        - @link CmdSetupPlayer() @c gogui-setup_player @endlink
        - @link CmdIsLegal() @c is_legal @endlink
        - @link CmdKomi() @c komi @endlink
        - @link CmdListStones() @c list_stones @endlink
        - @link CmdLoadSgf() @c loadsgf @endlink
        - @link CmdName() @c name @endlink
        - @link CmdPlaceFreeHandicap() @c place_free_handicap @endlink
        - @link CmdPlay() @c play @endlink
        - @link CmdRegGenMove() @c reg_genmove @endlink
        - @link CmdRegGenMoveToPlay() @c reg_genmove_toplay @endlink
        - @link CmdSaveSgf() @c savesgf @endlink
        - @link CmdSetFreeHandicap() @c set_free_handicap @endlink
        - @link CmdShowBoard() @c showboard @endlink
        - @link CmdTimeLastMove() @c time_last_move @endlink
        - @link CmdTimeLeft() @c time_left @endlink
        - @link CmdTimeSettings() @c time_settings @endlink
        - @link CmdUndo() @c undo @endlink
    */
    /** @name Command Callbacks */
    // @{
    // The callback functions are documented in the cpp file
    virtual void CmdAllLegal(GtpCommand&);
    virtual void CmdAllMoveValues(GtpCommand&);
    virtual void CmdAnalyzeCommands(GtpCommand&);
    virtual void CmdBoard(GtpCommand&);
    virtual void CmdBoardSize(GtpCommand&);
    virtual void CmdCheckPerformance(GtpCommand&);
    virtual void CmdClearBoard(GtpCommand&);
    virtual void CmdClock(GtpCommand&);
    virtual void CmdFinalScore(GtpCommand&);
    virtual void CmdFixedHandicap(GtpCommand&);
    virtual void CmdGenMove(GtpCommand&);
    virtual void CmdGetKomi(GtpCommand&);
    virtual void CmdGGUndo(GtpCommand&);
    virtual void CmdInterrupt(GtpCommand&);
    virtual void CmdIsLegal(GtpCommand&);
    virtual void CmdKomi(GtpCommand&);
    virtual void CmdLadder(GtpCommand&);
    virtual void CmdListStones(GtpCommand&);
    virtual void CmdLoadSgf(GtpCommand&);
    virtual void CmdName(GtpCommand&);
    virtual void CmdParam(GtpCommand&);
    virtual void CmdParamRules(GtpCommand&);
    virtual void CmdParamTimecontrol(GtpCommand&);
    virtual void CmdPlaceFreeHandicap(GtpCommand&);
    virtual void CmdPlay(GtpCommand&);
    virtual void CmdPlayerBoard(GtpCommand&);
    virtual void CmdPlaySequence(GtpCommand&);
    virtual void CmdPointNumbers(GtpCommand&);
    virtual void CmdPointInfo(GtpCommand&);
    virtual void CmdQuit(GtpCommand& cmd);
    virtual void CmdRegGenMove(GtpCommand&);
    virtual void CmdRegGenMoveToPlay(GtpCommand&);
    virtual void CmdRules(GtpCommand&);
    virtual void CmdSaveSgf(GtpCommand&);
    virtual void CmdSetFreeHandicap(GtpCommand&);
    virtual void CmdSetInfo(GtpCommand&);
    virtual void CmdSetup(GtpCommand&);
    virtual void CmdSetupPlayer(GtpCommand&);
    virtual void CmdShowBoard(GtpCommand&);
    virtual void CmdStaticLadder(GtpCommand&);
    virtual void CmdTimeLastMove(GtpCommand&);
    virtual void CmdTimeLeft(GtpCommand&);
    virtual void CmdTimeSettings(GtpCommand&);
    virtual void CmdUndo(GtpCommand&);
    // @} // @name

    GoGtpEngine(std::istream& in, std::ostream& out,
                const char* programPath = 0);

    ~GoGtpEngine();

    GoBook& Book();

    /** Write game and player boards. */
    void DumpState(std::ostream& out) const;

    /** Automatically save game after each move.
        @param prefix Filename prefix; game number and file extension sgf
        will be added.
    */
    void SetAutoSave(const std::string& prefix);

    /** Automatically write board to SgDebug() after changes.
        Default is false.
    */
    void SetAutoShowBoard(bool showBoard);

    /** Write everything written to SgDebug() during a genmove command to the
        comment in the new game node.
        During a genmove command, SgDebug() will be redirected to a string
        stream. After the move generation, SgDebug() will be reset to its old
        value and the string will be written to SgDebug(), as well as added
        as a comment to the new game node.
        Default is false.
    */
    void SetDebugToComment(bool debugToComment);

    /** Set maximum number of clear_board commands.
        -1 means no limit (default).
        After the limit is reached, the clear_board command will fail.
        This limit can be used to limit the game loop of the Computer
        bot interface to the Kiseido Go server http://kgs.kiseido.com.
    */
    void SetMaxClearBoard(int n);

    /** Set named rules.
        @param namedRules Named rules.
        @exception SgException on unknown rules.
        @see GoRules::SetNamedRules
    */
    void SetNamedRules(const std::string& namedRules);

    /** See TimeLimit() */
    void SetTimeLimit(double timeLimit);

    /** Time limit in seconds for move generation and other commands. */
    double TimeLimit();

#if GTPENGINE_PONDER
    /** Ponder.
        Calls GoPlayer::Ponder() if a player is set.
    */
    void Ponder();

    /** Stop pondering.
        Calls SgSetUserAbort()
    */
    void StopPonder();

    /** Stop pondering.
        Calls SgSetUserAbort(false)
    */
    void InitPonder();
#endif // GTPENGINE_PONDER

#if GTPENGINE_INTERRUPT
    /** Implementation of GtpEngine::Interrupt().
        Calls SgSetUserAbort()
    */
    void Interrupt();
#endif // GTPENGINE_INTERRUPT

protected:
    /** Current player.
        The player can be exchanged at runtime. It is null, if no player is
        set. This member is accessible by subclasses of GoGtpEngine to allow
        them to register external classes, which handle player specific GTP
        commands. These external classes can be constructed with a reference
        to this pointer, such that they can access the player they need.
        They have to check first, that the current player is the expected one.
    */
    GoPlayer* m_player;

    /** Hook function to be executed before each command.
        Resets user abort flag. Lengthy functions should poll SgUserAbort but
        should not reset the user abort flag themselves.
        Also flushes SgDebug() (see comment at BeforeWritingResponse()).
    */
    void BeforeHandleCommand();

    /** Hook function to be executed before the response of a command is
        written.
        Flushes SgDebug().
    */
    void BeforeWritingResponse();

    GoBoard& Board();

    const GoBoard& Board() const;

    void BoardChanged();

    void CheckLegal(std::string message, SgBlackWhite color, SgPoint move,
                    bool checkOnlyOccupied);

    void CheckMaxClearBoard();

    void CheckMoveStackOverflow() const;

    SgBlackWhite BlackWhiteArg(const GtpCommand& cmd,
                               std::size_t number) const;

    SgEmptyBlackWhite EmptyBlackWhiteArg(const GtpCommand& cmd,
                                         std::size_t number) const;

    SgPoint EmptyPointArg(const GtpCommand& cmd, std::size_t number) const;

    SgPoint GenMove(SgBlackWhite color);

    GoGame& GetGame();

    const GoGame& GetGame() const;

    SgPoint MoveArg(const GtpCommand& cmd, std::size_t number) const;

    /** Throws GtpFailure if no player set. */
    GoPlayer& Player() const;

    /** Check that number of arguments is one and get point argument. */
    SgPoint PointArg(const GtpCommand& cmd) const;

    SgPoint PointArg(const GtpCommand& cmd, std::size_t number) const;

    SgList<SgPoint> PointListArg(const GtpCommand& cmd,
                                 std::size_t number) const;

    SgList<SgPoint> PointListArg(const GtpCommand& cmd) const;

    /** Write integer array response to command.
        Elements with the value numeric_limits<int>::min() are considered to
        have no defined value and are written as "" for compatibility with
        GoGui.
    */
    void RespondNumberArray(GtpCommand& cmd, const SgPointArray<int>& array,
                            int scale);

    /** Set player.
        Takes ownership of player.
    */
    void SetPlayer(GoPlayer* player);

    SgPoint StoneArg(const GtpCommand& cmd, std::size_t number) const;

    void Init(int size);

    /** Play a move in game and goto new node.
        @throws GtpFailure If move stack overflow or illegal move.
    */
    void Play(SgBlackWhite color, SgPoint move);

    void RulesChanged();

private:
    /** Accept illegal ko or suicide moves in CmdPlay() */
    bool m_acceptIllegal;

    /** See SetAutoSave() */
    bool m_autoSave;

    /** See SetAutoShowBoard() */
    bool m_autoShowBoard;

    /** See SetDebugToComment() */
    bool m_debugToComment;

    /** Maximum number of clear_board commands.
        -1, if no limit.
        @see SetMaxClearBoard()
    */
    int m_maxClearBoard;

    /** Number of clear_board commands.
        @see SetMaxClearBoard()
    */
    int m_numberClearBoard;

    double m_timeLastMove;

    /** See GoGtpEngine::CmdTimeLimit */
    double m_timeLimit;

    /** Operator overhead time for time settings. */
    double m_overhead;

    /** Time settings for game. */
    GoGtpTimeSettings m_timeSettings;

    /** Default rules.
        Will be used whenever a new game is started.
    */
    GoRules m_defaultRules;

    GoBoard m_board;

    /** Order dependency: destruct m_game before m_board */
    GoGame m_game;

    SgGtpCommands m_sgCommands;

    GoBook m_book;

    GoBookCommands m_bookCommands;

    std::string m_autoSaveFileName;

    std::string m_autoSavePrefix;

    void AddPlayerProp(SgBlackWhite color, const std::string& name,
                       bool overwrite);

    void ApplyTimeSettings();

    void AutoSave() const;

    void CheckBoardEmpty() const;

    void CreateAutoSaveFileName();

    void GameFinished();

    void PlaceHandicap(const SgList<SgPoint>& stones);

    void RegisterCmd(const std::string& name,
                     GtpCallback<GoGtpEngine>::Method method);

    void SaveGame(const std::string& fileName) const;

    void Undo(int n);

    static void WriteBoardInfo(GtpCommand& cmd, const GoBoard& bd);
};

inline GoBoard& GoGtpEngine::Board()
{
    return m_board;
}

inline const GoBoard& GoGtpEngine::Board() const
{
    return m_board;
}

inline GoBook& GoGtpEngine::Book()
{
    return m_book;
}

inline GoGame& GoGtpEngine::GetGame()
{
    return m_game;
}

inline const GoGame& GoGtpEngine::GetGame() const
{
    return m_game;
}

inline void GoGtpEngine::SetDebugToComment(bool debugToComment)
{
    m_debugToComment = debugToComment;
}

inline void GoGtpEngine::SetMaxClearBoard(int n)
{
    m_maxClearBoard = n;
}

inline void GoGtpEngine::SetTimeLimit(double timeLimit)
{
    m_timeLimit = timeLimit;
}

inline double GoGtpEngine::TimeLimit()
{
    return m_timeLimit;
}

//----------------------------------------------------------------------------

/** Assertion handler that can be registered with SgRegisterAssertionHandler.
    Calls GoBoardUtil::DumpBoard with the engine's board.
*/
class GoGtpAssertionHandler
    : public SgAssertionHandler
{
public:
    GoGtpAssertionHandler(const GoGtpEngine& engine);

    void Run();

private:
    const GoGtpEngine& m_engine;
};

//----------------------------------------------------------------------------

#endif // GOGTPENGINE_H
