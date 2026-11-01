# Playing online

Two players, each with their own copy of the port, fight a 1-on-1 battle over the internet through the game's
Communication Mode. **Experimental:** it has been played between Windows and a Steam Deck, and the details may change.
How it works and how it was tested: [Communication Mode](communication-mode.md), sections 11 to 15.

Both players need the same version of the port, the same mods that change the game (music mods may differ) and the
same test settings. The game checks as the battle starts and says what differs. Saves and official DLC may differ.

## Host

1. Open the port's menu (Esc, or L3 + R3 on a controller) and go to the **Network** page.
2. Check that **Ad hoc play** is on, then choose **Host a session**.
3. The game asks your router to open its ports. After a moment a row under Play together shows your internet address,
   marked **Internet (NAT-PMP)** or **Internet (UPnP)**. Select it to copy the address, and send it to the other
   player.
4. The first time, Windows may ask whether to let the game use the network: allow it on private networks.

The host should be the player on a home connection. Mobile data cannot host (see below).

## Join

1. On the **Network** page, check that **Ad hoc play** is on.
2. Under Join a session, type the host's address in **Address** and confirm it.

## Then, in the game

Both players go to Communication Mode and into the same Online Lobby. One challenges the other; after fighter select
the host enters the battle. The battle finds its own way to the other player.

**Input delay** (Network page, Online battles): the frames between pressing a button and the fighter acting. The
host's setting counts. **Auto**, the default, sets it from the connection as the battle starts: 2 frames on a home
network, 3 at a round trip of about 75 to 100 ms, more when it is longer. A fixed number smooths a poor connection
(higher) or responds sooner (lower).

**Waiting.** If the other player's game falls silent, a band says "Waiting for the other player" and counts the
seconds. Hold B (or Esc) to leave the battle. After 30 seconds the game gives up by itself.

**Each player keeps their own camera settings.** The right-stick camera's dead zone, sensitivity and inverted axes
are sent across, so the other game turns your camera the way you set it.

Only the Online Lobby's 1-on-1 battles are supported. Group Battle and Communications are untested.

## If something goes wrong

**"This battle cannot start: the other player's game differs in ..."** The two games are not the same: update both
to the same release, and use the same mods that change the game.

**"The other player's game did not answer."** The battle's UDP port does not get through to the host. See the next
paragraphs.

**The row says the router did not open the ports.** Turn on UPnP or NAT-PMP in the router's settings, or forward
these ports on the router to the host's local address, which the Network page lists:

| Port | For |
| --- | --- |
| TCP 27312 | The lobby |
| TCP 27313 | The lobby's relay |
| UDP 27420 | The battle |

Then turn off **Open router ports** on the Network page, so the game stops asking.

**The row says your provider shares the address.** Your router has no address of its own on the internet: your
provider shares one among many customers (carrier-grade NAT, the rule on mobile data and common on some home
connections), or another router sits in front of yours. Opening ports does nothing then. The other player hosts
instead, or you both use a VPN.

**A VPN** puts both PCs on one private network, so no router is involved:
[Tailscale](https://tailscale.com/) or [ZeroTier](https://www.zerotier.com/). Both players install it and join the
same network. The host hosts as above; the joiner types the host's VPN address, which the host's Network page lists
too (Tailscale's start with `100.`).

**Something else blocks it.** A firewall program on the host other than Windows' own needs the same three ports
allowed.

## Not tested yet

- Two Windows PCs on separate machines (Windows against the Steam Deck, and two copies on one PC, were).
- The Steam Deck hosting: let the PC host.
- A router that only has UPnP (the one tested answered NAT-PMP first).
