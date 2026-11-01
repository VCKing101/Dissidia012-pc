# Playing online

Two players, each with their own copy of the port, fight a 1-on-1 battle over the internet through the game's
Communication Mode. **This is not in a release yet:** it works in test builds, between Windows and a Steam Deck, and
the details may change. How it works and how it was tested: [Communication Mode](communication-mode.md), sections 11
to 14.

Both players need the same version of the port. One player hosts; the other joins.

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

Both players go to Communication Mode and into the same online lobby. One challenges the other; after fighter select
the host enters the battle. The battle finds its own way to the other player, and the input delay is set from the
connection when the battle starts: 2 frames on a home network, 3 at a round trip of about 75 to 100 ms, more when it
is longer. A number in `online.input_delay` in `data/settings.ini` fixes it instead.

## If the other player cannot join

**The row says the router did not open the ports.** Turn on UPnP or NAT-PMP in the router's settings, or forward
these ports on the router to the host's local address, which the Network page lists:

| Port | For |
| --- | --- |
| TCP 27312 | The lobby |
| TCP 27313 | The lobby's relay |
| UDP 27420 | The battle |

Then turn off **Open router ports** on the Network page, so the game stops asking.

**The router opened the ports, but still nobody gets in.** Your provider may share one internet address among many
customers (carrier-grade NAT, the rule on mobile data and common on some home connections). Then your router itself
has no address on the internet, and opening its ports does nothing. Signs: the address the game shows starts with
`100.64` to `100.127`, `10.`, `172.16` to `172.31` or `192.168`, or differs from the one a "what is my IP" website
shows. The other player hosts instead, or you both use a VPN.

**A VPN** puts both PCs on one private network, so no router is involved:
[Tailscale](https://tailscale.com/) or [ZeroTier](https://www.zerotier.com/). Both players install it and join the
same network. The host hosts as above; the joiner types the host's VPN address, which the host's Network page lists
too (Tailscale's start with `100.`).

**Something else blocks it.** A firewall program on the host other than Windows' own, or a second router behind the
first, needs the same three ports allowed or forwarded by hand.

## Not yet

- Group Battle and the other Communication Mode rows are untested.
- The joining player types an address; there is no short join code and no server to find each other through.
