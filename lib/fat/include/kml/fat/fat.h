// SPDX-Licence-Identifier: X11
// Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#pragma once

#include <kml/base/type.h>

enum [[clang::enum_extensibility(closed)]] kml_fat_bytes_per_sector {
	KML_FAT_BYTES_PER_SECTOR_512 = 512,
	KML_FAT_BYTES_PER_SECTOR_1024 = 1024,
	KML_FAT_BYTES_PER_SECTOR_2048 = 2048,
	KML_FAT_BYTES_PER_SECTOR_4096 = 4096
};

enum [[clang::enum_extensibility(closed)]] kml_fat_sectors_per_cluster {
	KML_FAT_SECTORS_PER_CLUSTER_1 = 1,
	KML_FAT_SECTORS_PER_CLUSTER_2 = 2,
	KML_FAT_SECTORS_PER_CLUSTER_4 = 4,
	KML_FAT_SECTORS_PER_CLUSTER_8 = 8,
	KML_FAT_SECTORS_PER_CLUSTER_16 = 16,
	KML_FAT_SECTORS_PER_CLUSTER_32 = 32,
	KML_FAT_SECTORS_PER_CLUSTER_64 = 64,
	KML_FAT_SECTORS_PER_CLUSTER_128 = 128
};

enum [[clang::enum_extensibility(closed)]] kml_fat_media_descriptor {
	KML_FAT_MEDIA_DESCRIPTOR_F0 = 0xF0,
	KML_FAT_MEDIA_DESCRIPTOR_F8 = 0xF8,
	KML_FAT_MEDIA_DESCRIPTOR_F9 = 0xF9,
	KML_FAT_MEDIA_DESCRIPTOR_FA = 0xFA,
	KML_FAT_MEDIA_DESCRIPTOR_FB = 0xFB,
	KML_FAT_MEDIA_DESCRIPTOR_FC = 0xFC,
	KML_FAT_MEDIA_DESCRIPTOR_FD = 0xFD,
	KML_FAT_MEDIA_DESCRIPTOR_FE = 0xFE,
	KML_FAT_MEDIA_DESCRIPTOR_FF = 0xFF,

	KML_FAT_MEDIA_DESCRIPTOR_FIXED = KML_FAT_MEDIA_DESCRIPTOR_F8,
	KML_FAT_MEDIA_DESCRIPTOR_REMOVABLE = KML_FAT_MEDIA_DESCRIPTOR_F0
};

enum [[clang::enum_extensibility(closed)]] kml_fat_drive_number {
	KML_FAT_DRIVE_NUMBER_00 = 0x00,
	KML_FAT_DRIVE_NUMBER_80 = 0x80
};

#define KML_FAT_BPB16_EXTENDED_BOOT_SIGNATURE (0x29)

/*
 * NOTE: These filesystem type constants have a terminating `\0` where the
 * 		 on-disk structure does not.
 */
#define KML_FAT_BPB16_FILESYSTEM_FAT12 ("FAT12   ")
#define KML_FAT_BPB16_FILESYSTEM_FAT16 ("FAT16   ")
#define KML_FAT_BPB16_FILESYSTEM_FAT32 ("FAT32   ")

#define KML_FAT_BPB_SIGNATURE (0xAA55)

struct [[gnu::packed]] kml_fat_bpb16 {
	enum kml_fat_drive_number drive_number;
	kml_base_byte_t reserved0;
	kml_base_byte_t boot_signature;
	kml_base_u32_t volume_serial;
	char volume_label[11];
	char filesystem_type[8];
	kml_base_byte_t reserved1[448];
	kml_base_u16_t signature;
};

enum [[clang::enum_extensibility(closed)]] kml_fat_bpb32_mirroring {
	KML_FAT_BPB32_MIRRORED = 0,
	KML_FAT_BPB32_SINGLE = 1
};

enum [[clang::enum_extensibility(closed)]] kml_fat_bpb32_backup_sector {
	KML_FAT_BPB32_BACKUP_SECTOR_0 = 0,
	KML_FAT_BPB32_BACKUP_SECTOR_6 = 6,

	KML_FAT_BPB32_BACKUP_SECTOR_DISABLED = KML_FAT_BPB32_BACKUP_SECTOR_0,
	KML_FAT_BPB32_BACKUP_SECTOR_ENABLED = KML_FAT_BPB32_BACKUP_SECTOR_6
};

struct [[gnu::packed]] kml_fat_bpb32 {
	kml_base_u32_t sectors_per_fat_32;
	struct [[gnu::packed]] {
		kml_base_u8_t active_fat : 4;
		kml_base_u8_t reserved0 : 3;
		enum kml_fat_bpb32_mirroring mirroring : 1;
		kml_base_u8_t reserved1;
	} flags;
	kml_base_u8_t major_version;
	kml_base_u8_t minor_version;
	kml_base_u32_t root_cluster;
	kml_base_u16_t fsinfo_sector;
	enum kml_fat_bpb32_backup_sector backup_sector : sizeof(kml_base_u16_t) * KML_BASE_BYTE_BIT;
	kml_base_byte_t reserved0[12];
	struct kml_fat_bpb16 bpb16;
};

struct [[gnu::packed]] kml_fat_bpb {
	kml_base_byte_t jump_boot[3];
	char oem_name[8];
	enum kml_fat_bytes_per_sector bytes_per_sector : sizeof(kml_base_u16_t) * KML_BASE_BYTE_BIT;
	enum kml_fat_sectors_per_cluster sectors_per_cluster : KML_BASE_BYTE_BIT;
	kml_base_u16_t reserved_sector_count;
	kml_base_u8_t fat_count;
	// "For maximum compatibility, FAT16 volumes should use the value 512."
	kml_base_u16_t root_entry_count;
	kml_base_u16_t total_sectors_16;
	enum kml_fat_media_descriptor media_descriptor : KML_BASE_BYTE_BIT;
	kml_base_u16_t sectors_per_fat_16;
	kml_base_u16_t sectors_per_track;
	kml_base_u16_t head_count;
	kml_base_u32_t hidden_sectors;
	kml_base_u32_t total_sectors_32;

	union [[gnu::packed]] {
		struct kml_fat_bpb16 extended16;
		struct kml_fat_bpb32 extended32;
	};
};
