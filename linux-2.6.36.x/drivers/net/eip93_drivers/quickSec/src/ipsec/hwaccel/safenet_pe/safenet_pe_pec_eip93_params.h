/*
  safenet_pe_pec_eip93_params.h modified from safenet_pe_pec_1742_params.h

  Copyright:
  Copyright (c) 2009 SFNT Finland Oy.
  All rights reserved.

  Tunable configuration parameters for safenet_la.c and safenet_pe_pec.c
*/

#ifndef SAFENET_PE_PEC_EIP93_PARAMS_H
#define SAFENET_PE_PEC_EIP93_PARAMS_H


#define SAFENET_PE_PLATFORM_PE_REQUIRES_SWAP /*for integration*/
#define SAFENET_PE_PLATFORM_SSH_SAFENET_MAX_QUEUED_OPERATIONS 390
//#define SAFENET_PE_PLATFORM_SSH_SAFENET_POLLING /*for integration*/
/*#define SAFENET_PE_PLATFORM_SSH_SAFENET_OCM_SA*/
#define SAFENET_PE_PLATFORM_SSH_SAFENET_NOT_COHERENT_CACHE 
/*#define SAFENET_PE_PLATFORM_SSH_SAFENET_PE_SA_CACHING*/
#define SAFENET_PE_PLATFORM_SSH_SAFENET_PDR_ENTRIES_PER_INTERRUPT 16
#define SAFENET_PE_PLATFORM_SSH_SAFENET_PDR_GET_COUNT		\
  (SAFENET_PE_PLATFORM_SSH_SAFENET_MAX_QUEUED_OPERATIONS + 9)

/* This setting must be less or equal than 
 ADAPTER_EIP94V2_RINGSIZE_BYTES/32 used
 in the cs_adapter.h file of the 1742_V-Driver */
#define SAFENET_PE_PLATFORM_SSH_SAFENET_PDR_BURST_COUNT 1


#ifdef SAFENET_PE_PLATFORM_PE_REQUIRES_SWAP
  #define SAFENET_PE_PLATFORM_SSH_SAFENET_MIN_BYTE_SWAP
#endif



#endif /* SAFENET_PE_PEC_EIP93_PARAMS_H */
