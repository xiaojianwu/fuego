//----------------------------------------------------------------------------
/** @file GoUctBookBuilderCommands.h
*/
//----------------------------------------------------------------------------

#ifndef GOUCT_BOOKBUILDERCOMMANDS_H
#define GOUCT_BOOKBUILDERCOMMANDS_H

#include <string>
#include <typeinfo>
#include "GtpEngine.h"
#include "GoUctPlayoutPolicy.h"
#include "GoUctGlobalSearch.h"
#include "GoUctPlayer.h"
#include "GoUctBookBuilder.h"

class GoBoard;
class GoPlayer;
class GoUctBoard;
class GoUctSearch;

//----------------------------------------------------------------------------

template<class PLAYER>
class GoUctBookBuilderCommands
{
public:
    /** Constructor.
        @param bd The game board.
        @param player Reference to pointer to current player, this player can
        be null or a different player, but those commands of this class that
        need a GoUctPlayer will fail, if the current player is not a
        PLAYER.
    */
    GoUctBookBuilderCommands(GoBoard& bd, GoPlayer*& player);

    void AddGoGuiAnalyzeCommands(GtpCommand& cmd);

    /** @page gouctbookbuildergtpcommands GoUctBookBuilderCommands Commands
        - @link CmdOpen() @c autobook_open @endlink
        - @link CmdClose() @c autobook_close @endlink
        - @link CmdExpand() @c autobook_expand @endlink
        - @link CmdRefresh() @c autobook_refresh @endlink
        - @link CmdParam()  @c autobook_param @endlink
        - @link CmdScores() @c autobook_scores @endlink
    */
    /** @name Command Callbacks */
    // @{
    void CmdOpen(GtpCommand& cmd);
    void CmdClose(GtpCommand& cmd);
    void CmdExpand(GtpCommand& cmd);
    void CmdRefresh(GtpCommand& cmd);
    void CmdParam(GtpCommand& cmd);
    void CmdScores(GtpCommand& cmd);
    // @} // @name

    void Register(GtpEngine& engine);

private:
    GoBoard& m_bd;

    GoPlayer*& m_player;

    boost::scoped_ptr<GoAutoBook> m_book;

    GoUctBookBuilder<PLAYER> m_bookBuilder;

    PLAYER& Player();

    void Register(GtpEngine& e, const std::string& command,
                typename GtpCallback<GoUctBookBuilderCommands>::Method method);
};

//----------------------------------------------------------------------------

template<class PLAYER>
GoUctBookBuilderCommands<PLAYER>
::GoUctBookBuilderCommands(GoBoard& bd, GoPlayer*& player)
    : m_bd(bd),
      m_player(player),
      m_book(0),
      m_bookBuilder(bd)
{
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::AddGoGuiAnalyzeCommands(GtpCommand& cmd)
{
    cmd << 
        "none/AutoBook Close/autobook_close\n"
        "none/AutoBook Expand/autobook_expand\n"
        "none/AutoBook Open/autobook_open %r\n"
        "param/AutoBook Param/autobook_param\n"
        "none/AutoBook Refresh/autobook_refresh\n"
        "gfx/AutoBook Scores/autobook_scores\n";
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::Register(GtpEngine& e)
{
    Register(e, "autobook_close", &GoUctBookBuilderCommands<PLAYER>::CmdClose);
    Register(e, "autobook_expand", 
             &GoUctBookBuilderCommands<PLAYER>::CmdExpand);
    Register(e, "autobook_open", &GoUctBookBuilderCommands<PLAYER>::CmdOpen);
    Register(e, "autobook_param", &GoUctBookBuilderCommands<PLAYER>::CmdParam);
    Register(e, "autobook_refresh", 
             &GoUctBookBuilderCommands<PLAYER>::CmdRefresh);
    Register(e, "autobook_scores", 
             &GoUctBookBuilderCommands<PLAYER>::CmdScores);
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::
Register(GtpEngine& engine, const std::string& command,
         typename GtpCallback<GoUctBookBuilderCommands>::Method method)
{
    engine.Register(command, 
                    new GtpCallback<GoUctBookBuilderCommands>(this, method));
}

template<class PLAYER>
PLAYER& GoUctBookBuilderCommands<PLAYER>::Player()
{
    if (m_player == 0)
        throw GtpFailure("player not GoUctPlayer");
    try
    {
        return dynamic_cast<PLAYER&>(*m_player);
    }
    catch (const std::bad_cast&)
    {
        throw GtpFailure("player not of right type!");
    }
}
//----------------------------------------------------------------------------

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdOpen(GtpCommand& cmd)
{
    cmd.CheckNuArg(1);
    m_book.reset(new GoAutoBook(cmd.Arg(0)));
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdClose(GtpCommand& cmd)
{
    cmd.CheckArgNone();
    m_book.reset(0);
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdExpand(GtpCommand& cmd)
{
    if (m_book.get() == 0)
        throw GtpFailure() << "No opened auto-book!\n";
    cmd.CheckNuArg(1);
    int numExpansions = cmd.IntArg(0, 1);
    m_bookBuilder.SetPlayer(Player());
    m_bookBuilder.SetState(*m_book);
    m_bookBuilder.Expand(numExpansions);
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdRefresh(GtpCommand& cmd)
{
    if (m_book.get() == 0)
        throw GtpFailure() << "No opened auto-book!\n";
    cmd.CheckArgNone();
    m_bookBuilder.SetPlayer(Player());
    m_bookBuilder.SetState(*m_book);
    m_bookBuilder.Refresh();
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdParam(GtpCommand& cmd)
{
    if (cmd.NuArg() == 0)
    {
        cmd << "[bool] use_widening " << m_bookBuilder.UseWidening() << '\n'
            << "[string] alpha " << m_bookBuilder.Alpha() << '\n'
            << "[string] expand_width " << m_bookBuilder.ExpandWidth() << '\n'
            << "[string] expand_threshold " 
            << m_bookBuilder.ExpandThreshold() << '\n'
            << "[string] num_threads " << m_bookBuilder.NumThreads() << '\n'
            << "[string] num_games_per_evaluation " 
            << m_bookBuilder.NumGamesPerEvaluation() << '\n'
            << "[string] num_games_per_sort "
            << m_bookBuilder.NumGamesPerSort() << '\n';
    }
    else if (cmd.NuArg() == 2)
    {
        std::string name = cmd.Arg(0);
        if (name == "num_threads")
            m_bookBuilder.SetNumThreads(cmd.IntArg(1, 1));
        else if (name == "num_games_per_evaluation")
            m_bookBuilder.SetNumGamesPerEvaluation(cmd.SizeTypeArg(1, 1));
        else if (name == "num_games_per_sort")
            m_bookBuilder.SetNumGamesPerSort(cmd.SizeTypeArg(1, 1));
        else if (name == "use_widening")
            m_bookBuilder.SetUseWidening(cmd.BoolArg(1));
        else if (name == "expand_width")
            m_bookBuilder.SetExpandWidth(cmd.IntArg(1, 1));
        else if (name == "expand_threshold")
            m_bookBuilder.SetExpandThreshold(cmd.IntArg(1, 1));
        else if (name == "alpha")
        {
            float alpha = cmd.FloatArg(1);
            if (alpha < 0)
                throw GtpFailure("Alpha must be greater than 0!");
            m_bookBuilder.SetAlpha(alpha);
        }
    }
    else
        throw GtpFailure() << "Expected 0 or 2 arguments!\n";
}

template<class PLAYER>
void GoUctBookBuilderCommands<PLAYER>::CmdScores(GtpCommand& cmd)
{
    cmd.CheckArgNone();
    GoBookState state(m_bd);
    state.Synchronize();
    cmd << "INFLUENCE ";
    for (GoBoard::Iterator it(m_bd); it; ++it)
    {
        if (m_bd.IsLegal(*it))
        {
            state.Play(*it);
            SgBookNode node;
            if (m_book->Get(state, node))
            {
                float value 
                    = m_bookBuilder.InverseEval(m_bookBuilder.Value(node));
                float scaledValue = (value * 2 - 1);
                if (m_bd.ToPlay() != SG_BLACK)
                    scaledValue *= -1;
                cmd << ' ' << SgWritePoint(*it) << ' ' << scaledValue;
            }
            state.Undo();
        }
    }
    cmd << "\nLABEL ";
    for (GoBoard::Iterator it(m_bd); it; ++it)
    {
        if (m_bd.IsLegal(*it))
        {
            state.Play(*it);
            SgBookNode node;
            if (m_book->Get(state, node))
                cmd << ' ' << SgWritePoint(*it) << ' ' << node.m_count;
            state.Undo();
        }
    }
    cmd << '\n';
}

//----------------------------------------------------------------------------

#endif // GOUCT_BOOKBUILDERCOMMANDS_H
