#ifndef __CURL_LDAP_H
#define __CURL_LDAP_H

/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2007, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: //WIFI_SOC/release/SDK_4_1_0_0/source/user/curl/lib/curl_ldap.h#1 $
 ***************************************************************************/
#ifndef CURL_DISABLE_LDAP
extern const struct Curl_handler Curl_handler_ldap;

#ifdef HAVE_LDAP_SSL
extern const struct Curl_handler Curl_handler_ldaps;
#endif

#endif
#endif /* __CURL_LDAP_H */
