// Copyright (C) 2004 WoW Daemon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "StdAfx.h"

void WorldSession::HandleAttackSwingOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE(recv_data, 8);
    uint64 guid;
    recv_data >> guid;

    if(!guid)
    {
        // does this mean cancel combat?
        HandleAttackStopOpcode(recv_data);
        return;
    }

    // AttackSwing
    sLog.outDebug( "WORLD: Recvd CMSG_ATTACKSWING Message" );

    if(GetPlayer()->IsPacified() || GetPlayer()->IsStunned())
        return;

//    printf("Got ATTACK SWING: %08X %08X\n", GUID_HIPART(guid), GUID_LOPART(guid));
    Unit *pEnemy = World::GetUnit(guid);
    //printf("Pointer: %08X\n", pEnemy);

    if(!pEnemy)
    {
        sLog.outDebug("WORLD: "I64FMT" does not exist.", guid);
        return;
    }

    GetPlayer()->smsg_AttackStart(pEnemy);
    GetPlayer()->EventAttackStart();

    // Set PVP Flag.
    if(pEnemy->IsPlayer() && isHostile(_player, pEnemy))
        _player->SetPvPFlag();
}

void WorldSession::HandleAttackStopOpcode( WorldPacket & recv_data )
{
    uint64 guid = GetPlayer()->GetSelection();
    Unit *pEnemy = NULL;

    if(guid)
    {
        pEnemy = World::GetUnit(guid);
    }
    if(!pEnemy)
    {
        sLog.outError( "WORLD: "I64FMT" doesn't exist.",guid);
        return;
    }

    GetPlayer()->EventAttackStop();
    GetPlayer()->smsg_AttackStop(pEnemy);
}
