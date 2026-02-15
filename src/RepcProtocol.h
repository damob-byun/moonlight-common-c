#pragma once

#include <stdint.h>

#ifdef _WIN32
#define REPC_PACKED
#pragma pack(push, 1)
#else
#define REPC_PACKED __attribute__((packed))
#endif

// Bitrate request packet (Client -> Server)
// Sent on control stream when client detects network quality changes
typedef struct _SS_BITRATE_REQUEST
{
    uint32_t requestedBitrateKbps; // BE: requested bitrate in Kbps
    uint8_t reason;                // REPC_BITRATE_REASON_* value
    uint8_t currentLossPercent;    // Current packet loss rate (0-100)
    uint16_t currentRttMs;         // BE: current RTT in milliseconds
} REPC_PACKED SS_BITRATE_REQUEST, *PSS_BITRATE_REQUEST;

// Bitrate acknowledgement packet (Server -> Client)
// Sent after server applies bitrate change
typedef struct _SS_BITRATE_ACK
{
    uint32_t appliedBitrateKbps; // BE: actual applied bitrate
    uint32_t maxBitrateKbps;     // BE: server maximum allowed bitrate
} REPC_PACKED SS_BITRATE_ACK, *PSS_BITRATE_ACK;

// Audio state notification (Server -> Client)
// Sent when server detects silence/audio activity transitions
typedef struct _SS_AUDIO_STATE
{
    uint8_t state; // REPC_AUDIO_STATE_SILENT or REPC_AUDIO_STATE_ACTIVE
    uint8_t reserved[3];
} REPC_PACKED SS_AUDIO_STATE, *PSS_AUDIO_STATE;

// Cursor position update (Server -> Client)
// Sent per-frame or on cursor position change
typedef struct _SS_CURSOR_POSITION
{
    uint16_t x;            // BE: cursor X in screen coordinates
    uint16_t y;            // BE: cursor Y in screen coordinates
    uint16_t screenWidth;  // BE: capture screen width
    uint16_t screenHeight; // BE: capture screen height
    uint8_t visible;       // Non-zero if cursor is visible
    uint8_t shapeId;       // Current cursor shape ID (for change detection)
    uint8_t reserved[2];
} REPC_PACKED SS_CURSOR_POSITION, *PSS_CURSOR_POSITION;

// Cursor shape header (Server -> Client)
// Sent only when cursor shape changes. Followed by variable-length pixel data.
typedef struct _SS_CURSOR_SHAPE
{
    uint8_t shapeId;     // Shape identifier (matches SS_CURSOR_POSITION.shapeId)
    uint8_t type;        // 0=RGBA, 1=monochrome, 2=masked color
    uint16_t width;      // BE: cursor image width in pixels
    uint16_t height;     // BE: cursor image height in pixels
    uint16_t hotspotX;   // BE: hotspot X offset from top-left
    uint16_t hotspotY;   // BE: hotspot Y offset from top-left
    uint32_t dataLength; // BE: length of following RGBA pixel data in bytes
    // Followed by: uint8_t pixelData[dataLength]
} REPC_PACKED SS_CURSOR_SHAPE, *PSS_CURSOR_SHAPE;

// Cursor shape types
#define REPC_CURSOR_TYPE_RGBA 0
#define REPC_CURSOR_TYPE_MONOCHROME 1
#define REPC_CURSOR_TYPE_MASKED 2

// Clipboard transfer packet (bidirectional: Server <-> Client)
// Sent when clipboard content changes. Followed by variable-length UTF-8 text.
// Client->Server uses CS_CLIPBOARD_PTYPE (0x550B)
// Server->Client uses SS_CLIPBOARD_PTYPE (0x550C)
typedef struct _SS_CLIPBOARD
{
    uint32_t textLength; // BE: length of following UTF-8 text in bytes (0 = clear clipboard)
    // Followed by: uint8_t text[textLength]
} REPC_PACKED SS_CLIPBOARD, *PSS_CLIPBOARD;

#ifdef _WIN32
#pragma pack(pop)
#endif
#undef REPC_PACKED
