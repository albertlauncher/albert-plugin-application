// Copyright (c) 2025 Manuel Schneider

#include "plugin.h"
#include <albert/albert.h>
#include <albert/albert.h>
#include <albert/logging.h>
#include <albert/matcher.h>
#include <albert/standarditem.h>
#include <albert/systemutil.h>
ALBERT_LOGGING_CATEGORY("albert")
using namespace Qt::Literals;
using namespace albert::util;
using namespace albert;
using namespace std;

Plugin::Plugin():
    apps_plugin{"applications"_L1},
    strings({
        .cache=tr("Cache location"),
        .cached=tr("Albert cache location"),
        .config=tr("Config location"),
        .configd=tr("Albert config location"),
        .data=tr("Data location"),
        .datad=tr("Albert data location"),
        .open=tr("Open"),
        .topen=tr("Open in terminal"),
        .quit=tr("Quit"),
        .quitd=tr("Quit Albert"),
        .restart=tr("Restart"),
        .restartd=tr("Restart Albert"),
        .settings=tr("Settings"),
        .settingsd=tr("Albert settings"),
    })
{}

void Plugin::openTermAt(const std::filesystem::path &loc) const
{ apps_plugin->runTerminal("cd '%1'; exec $SHELL"_L1.arg(QString::fromLocal8Bit(loc.c_str()))); }

vector<RankItem> Plugin::handleGlobalQuery(const Query &query)
{
    static const QStringList icon_urls{":app_icon"_L1};
    Matcher matcher(query.string());
    vector<RankItem> rank_items;
    Match m;

    if (m = matcher.match(strings.settings); m)
        rank_items.emplace_back(StandardItem::make(u"sett"_s,
                                                   strings.settings,
                                                   strings.settingsd,
                                                   icon_urls,
                                                   {{"open"_L1,
                                                     strings.open,
                                                     [] { showSettings(); }}}),
                                m);

    if (m = matcher.match(strings.quit); m)
        rank_items.emplace_back(StandardItem::make(u"quit"_s,
                                                   strings.quit,
                                                   strings.quitd,
                                                   icon_urls,
                                                   {{"quit"_L1,
                                                     strings.quit,
                                                     [] { quit(); }}}),
                                m);

    if (m = matcher.match(strings.restart); m)
        rank_items.emplace_back(StandardItem::make(u"restart"_s,
                                                   strings.restart,
                                                   strings.restartd,
                                                   icon_urls,
                                                   {{"restart"_L1,
                                                     strings.restart,
                                                     [] { restart(); }}}),
                                m);

    if (m = matcher.match(strings.cache); m)
    {
        vector<Action> actions = {{"open"_L1, strings.open, [] { open(albert::cacheLocation()); }}};
        if (apps_plugin)
            actions.emplace_back("term"_L1, strings.topen,
                                 [this]{ openTermAt(albert::cacheLocation()); });

        auto i = rank_items.emplace_back(StandardItem::make(u"cache"_s,
                                                            strings.cache,
                                                            strings.cached,
                                                            icon_urls,
                                                            actions),
                                         m);
    }

    if (m = matcher.match(strings.config); m)
    {
        vector<Action> actions = {{"open"_L1, strings.open, [] { open(albert::configLocation()); }}};
        if (apps_plugin)
            actions.emplace_back("term"_L1, strings.topen,
                                 [this]{ openTermAt(albert::configLocation()); });

        auto i = rank_items.emplace_back(StandardItem::make(u"config"_s,
                                                            strings.config,
                                                            strings.configd,
                                                            icon_urls,
                                                            actions),
                                         m);
    }

    if (m = matcher.match(strings.data); m)
    {
        vector<Action> actions = {{"open"_L1, strings.open, [] { open(albert::dataLocation()); }}};
        if (apps_plugin)
            actions.emplace_back("term"_L1, strings.topen,
                                 [this]{ openTermAt(albert::dataLocation()); });

        auto i = rank_items.emplace_back(StandardItem::make(u"data"_s,
                                                            strings.data,
                                                            strings.datad,
                                                            icon_urls,
                                                            actions),
                                         m);
    }

    return rank_items;
}

QString Plugin::defaultTrigger() const { return u"albert "_s; }
