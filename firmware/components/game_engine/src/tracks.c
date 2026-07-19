#include "tracks.h"
#include "sound.h"
#include <stdint.h>

// --- Intro Section ---
const Note_t mario_intro[] = {{NOTE_E5, DURATION_EIGHTH},
                              {NOTE_E5, DURATION_EIGHTH},
                              {NOTE_REST, DURATION_EIGHTH},
                              {NOTE_E5, DURATION_EIGHTH},
                              {NOTE_REST, DURATION_EIGHTH},
                              {NOTE_C5, DURATION_EIGHTH},
                              {NOTE_E5, DURATION_EIGHTH},
                              {NOTE_REST, DURATION_EIGHTH},
                              {NOTE_G5, DURATION_EIGHTH},
                              {NOTE_REST, DURATION_QUARTER},
                              {NOTE_REST, DURATION_EIGHTH},
                              {NOTE_G4, DURATION_EIGHTH},
                              {NOTE_REST, DURATION_QUARTER},
                              {NOTE_REST, DURATION_EIGHTH},
                              {0, DURATION_END}};

// --- Main Verse Section (Part 1) ---
const Note_t mario_verse_1[] = {{NOTE_C5, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_QUARTER},
                                {NOTE_G4, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_QUARTER},
                                {NOTE_E4, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_QUARTER},
                                {NOTE_A4, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_EIGHTH},
                                {NOTE_B4, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_EIGHTH},
                                {NOTE_AS4, DURATION_EIGHTH},
                                {NOTE_A4, DURATION_EIGHTH},
                                {0, DURATION_END}};

// --- Main Verse Section (Part 2) ---
const Note_t mario_verse_2[] = {{NOTE_G4, DURATION_EIGHTH},
                                {NOTE_E5, DURATION_EIGHTH},
                                {NOTE_G5, DURATION_EIGHTH},
                                {NOTE_A5, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_EIGHTH},
                                {NOTE_F5, DURATION_EIGHTH},
                                {NOTE_G5, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_EIGHTH},
                                {NOTE_E5, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_EIGHTH},
                                {NOTE_C5, DURATION_EIGHTH},
                                {NOTE_D5, DURATION_EIGHTH},
                                {NOTE_B4, DURATION_EIGHTH},
                                {NOTE_REST, DURATION_QUARTER},
                                {0, DURATION_END}};

// --- Underworld Bridge Section (Part 1) ---
const Note_t mario_bridge_1[] = {
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_G5, DURATION_EIGHTH},
    {NOTE_FS5, DURATION_EIGHTH},  {NOTE_F5, DURATION_EIGHTH},
    {NOTE_DS5, DURATION_EIGHTH},  {NOTE_REST, DURATION_EIGHTH},
    {NOTE_E5, DURATION_EIGHTH},   {NOTE_REST, DURATION_EIGHTH},
    {NOTE_GS4, DURATION_EIGHTH},  {NOTE_A4, DURATION_EIGHTH},
    {NOTE_C5, DURATION_EIGHTH},   {NOTE_REST, DURATION_EIGHTH},
    {NOTE_A4, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_D5, DURATION_EIGHTH},   {0, DURATION_END}};

// --- Underworld Bridge Section (Part 2: Treble Jump) ---
const Note_t mario_bridge_2_jump[] = {{NOTE_REST, DURATION_EIGHTH},
                                      {NOTE_G5, DURATION_EIGHTH},
                                      {NOTE_FS5, DURATION_EIGHTH},
                                      {NOTE_F5, DURATION_EIGHTH},
                                      {NOTE_DS5, DURATION_EIGHTH},
                                      {NOTE_REST, DURATION_EIGHTH},
                                      {NOTE_E5, DURATION_EIGHTH},
                                      {NOTE_REST, DURATION_EIGHTH},
                                      {NOTE_C6, DURATION_EIGHTH},
                                      {NOTE_REST, DURATION_EIGHTH},
                                      {NOTE_C6, DURATION_EIGHTH},
                                      {NOTE_C6, DURATION_EIGHTH},
                                      {NOTE_REST, DURATION_QUARTER},
                                      {NOTE_REST, DURATION_EIGHTH},
                                      {0, DURATION_END}};

// --- Underworld Bridge Section (Part 3: Descent Outro) ---
const Note_t mario_bridge_3_outro[] = {{NOTE_DS5, DURATION_EIGHTH},
                                       {NOTE_REST, DURATION_QUARTER},
                                       {NOTE_D5, DURATION_EIGHTH},
                                       {NOTE_REST, DURATION_QUARTER},
                                       {NOTE_C5, DURATION_EIGHTH},
                                       {NOTE_REST, DURATION_HALF},
                                       {0, DURATION_END}};

// --- Steel Drum Hook Section ---
const Note_t mario_hook[] = {
    {NOTE_C5, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_D5, DURATION_EIGHTH},   {NOTE_REST, DURATION_EIGHTH},
    {NOTE_E5, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_A4, DURATION_EIGHTH},
    {NOTE_G4, DURATION_EIGHTH},   {NOTE_REST, DURATION_QUARTER},
    {NOTE_REST, DURATION_EIGHTH},

    {NOTE_C5, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_D5, DURATION_EIGHTH},   {NOTE_E5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_HALF},   {NOTE_REST, DURATION_QUARTER},

    {NOTE_C5, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_C5, DURATION_EIGHTH},
    {NOTE_D5, DURATION_EIGHTH},   {NOTE_REST, DURATION_EIGHTH},
    {NOTE_E5, DURATION_EIGHTH},   {NOTE_C5, DURATION_EIGHTH},
    {NOTE_REST, DURATION_EIGHTH}, {NOTE_A4, DURATION_EIGHTH},
    {NOTE_G4, DURATION_EIGHTH},   {0, DURATION_END}};

// Index map for phrases
const Note_t *mario_phrases[] = {
    mario_intro,          // 0
    mario_verse_1,        // 1
    mario_verse_2,        // 2
    mario_bridge_1,       // 3
    mario_bridge_2_jump,  // 4
    mario_bridge_3_outro, // 5
    mario_hook            // 6
};

// The sequence mapping out the accurate full layout of the song
const uint8_t mario_order[] = {
    0,          // Intro
    1, 2, 1, 2, // Main Verse played twice
    3, 4, 3, 5, // Underworld Bridge complete loop
    1, 2, 1, 2, // Main Verse repeats twice again
    6           // Hook section
};

const Track_t mario = {
    .order = mario_order,
    .order_len = sizeof(mario_order) / sizeof(mario_order[0]),
    .phrases = mario_phrases,

};