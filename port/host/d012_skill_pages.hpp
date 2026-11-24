#pragma once

// The 10 Skills cheat (d012_skill_pages.cpp): Story Mode's Skills on two
// pages, L2 or R2 to turn the page. Its overrides install themselves; this
// adds the page hint drawn by the Skill lists.

namespace d012::skill_pages {

// The hint ("L2/R2  Page 1/2") over the game beside the Skill bar and the
// shop's throw-away window. Once, before the game runs.
void install();

} // namespace d012::skill_pages
