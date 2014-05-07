/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef VP8RTPFMT_H
#define VP8RTPFMT_H

#include <mediastreamer2/mscommon.h>
#include <mediastreamer2/msfilter.h>
#include <mediastreamer2/msqueue.h>

/**
 * This file declares an API useful to pack/unpack a VP8 stream in RTP packets
 * as described in draft-ietf-payload-vp8-11
 * (http://tools.ietf.org/html/draft-ietf-payload-vp8-11)
 */

#ifdef __cplusplus
extern "C"{
#endif

	typedef enum Vp8RtpFmtErrorCode {
		Vp8RtpFmtOk = 0,
		Vp8RtpFmtInvalidPayloadDescriptor = -1,
		Vp8RtpFmtIncompleteFrame = -2,
		Vp8RtpFmtIncompletePartition = -3,
		Vp8RtpFmtInvalidFrame = -4,
		Vp8RtpFmtInvalidPartition = -5
	} Vp8RtpFmtErrorCode;

	typedef struct Vp8RtpFmtPayloadDescriptor {
		uint16_t pictureid;
		uint8_t pid;
		uint8_t tl0picidx;
		uint8_t tid;
		uint8_t keyidx;
		bool_t extended_control_bits_present;
		bool_t non_reference_frame;
		bool_t start_of_partition;
		bool_t pictureid_present;
		bool_t tl0picidx_present;
		bool_t tid_present;
		bool_t keyidx_present;
		bool_t layer_sync;
	} Vp8RtpFmtPayloadDescriptor;

	typedef struct Vp8RtpFmtPacket {
		mblk_t *m;
		Vp8RtpFmtPayloadDescriptor *pd;
		uint32_t extended_cseq;
		Vp8RtpFmtErrorCode error;
		bool_t last_packet_of_frame;
	} Vp8RtpFmtPacket;

	typedef struct Vp8RtpFmtPartition {
		MSList *packets_list;
		Vp8RtpFmtErrorCode error;
		mblk_t *m;
		bool_t last_partition_of_frame;
		bool_t outputted;
	} Vp8RtpFmtPartition;

	typedef struct Vp8RtpFmtFrame {
		MSList *partitions_list;
		Vp8RtpFmtErrorCode error;
		bool_t outputted;
		bool_t discarded;
	} Vp8RtpFmtFrame;


	typedef struct Vp8RtpFmtUnpackerCtx {
		MSFilter *filter;
		MSList *frames_list;
		MSQueue output_queue;
		uint32_t last_ts;
		uint32_t ref_cseq;
		bool_t initialized_last_ts;
		bool_t initialized_ref_cseq;
	} Vp8RtpFmtUnpackerCtx;

	typedef struct Vp8RtpFmtPackerCtx {
		MSQueue *output_queue;
		uint8_t nb_partitions;
	} Vp8RtpFmtPackerCtx;


	void vp8rtpfmt_packer_init(Vp8RtpFmtPackerCtx *ctx, uint8_t nb_partitions);
	void vp8rtpfmt_packer_uninit(Vp8RtpFmtPackerCtx *ctx);
	void vp8rtpfmt_packer_process(Vp8RtpFmtPackerCtx *ctx, MSList *in, MSQueue *out);

	void vp8rtpfmt_unpacker_init(Vp8RtpFmtUnpackerCtx *ctx, MSFilter *f);
	void vp8rtpfmt_unpacker_uninit(Vp8RtpFmtUnpackerCtx *ctx);
	void vp8rtpfmt_unpacker_process(Vp8RtpFmtUnpackerCtx *ctx, MSQueue *inout);
	uint32_t vp8rtpfmt_unpacker_calc_extended_cseq(Vp8RtpFmtUnpackerCtx *ctx, uint16_t cseq);

#ifdef __cplusplus
}
#endif

#endif /* VP8RTPFMT_H */