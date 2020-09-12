#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <stddef.h>

enum StatIndex
{
    STAT_FRAME,
    STAT_COMPILE,
    STAT_EXECUTE,
    STAT_COLLIDE,
    STAT_RENDER,
    STAT_REHASH,
    STAT_LOAD,
    STAT_RECORD,
    STAT_COUNT,
};

#define NUM_STAT_FRAMES 100

extern uint64_t all_stats[STAT_COUNT][NUM_STAT_FRAMES];
extern size_t current_stat_frame;

static inline void stats_add(StatIndex st, uint64_t time)
{
    all_stats[st][current_stat_frame] += time;
}

static inline uint64_t stats_get(StatIndex st, int frames_ago)
{
    int frame = current_stat_frame - frames_ago;
    if(frame < 0) frame += NUM_STAT_FRAMES;
    if(frame < 0) frame = 0;
    return all_stats[st][frame];
}

static inline uint64_t stats_get_current(StatIndex st)
{
    return stats_get(st, 0);
}

static inline void stats_next_frame()
{
    current_stat_frame++;
    if(current_stat_frame == NUM_STAT_FRAMES)
    {
        current_stat_frame = 0;
    }
    for(size_t i = 0; i < STAT_COUNT; i++)
    {
        all_stats[i][current_stat_frame] = 0;
    }
}

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
static inline uint64_t get_time_ns()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    return tp.tv_sec * 1000000000 + tp.tv_nsec;
}
#else
#include <SDL.h>
static inline uint64_t get_time_ns()
{
    return SDL_GetTicks() * 1000000;
}
#endif

#endif // STATS_H
