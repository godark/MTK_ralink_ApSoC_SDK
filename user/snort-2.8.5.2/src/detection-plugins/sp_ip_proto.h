/*
** Copyright (C) 2002-2009 Sourcefire, Inc.
** Copyright (C) 1998-2002 Martin Roesch <roesch@sourcefire.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* $Id: //WIFI_SOC/release/SDK_4_1_0_0/source/user/snort-2.8.5.2/src/detection-plugins/sp_ip_proto.h#1 $ */

#ifndef __SP_IP_PROTO_H__
#define __SP_IP_PROTO_H__

#include "rules.h"
#include "sf_types.h"

void SetupIpProto(void);
uint32_t IpProtoCheckHash(void *);
int IpProtoCheckCompare(void *, void *);
int GetOtnIpProto(OptTreeNode *);
int GetIpProtos(void *, uint8_t *, int);

#endif  /* __SP_IP_PROTO_H__ */