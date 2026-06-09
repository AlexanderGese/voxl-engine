#include "jobsys_chain.h"

void jobsys_chain_pool_init(jobsys_chain_pool *p) {
    // thread every slot onto the freelist, last slot points at -1.
    for (int i = 0; i < JOBSYS_MAX_CHAIN; i++) {
        p->slots[i].in_use = 0;
        p->free_next[i] = i + 1;
    }
    p->free_next[JOBSYS_MAX_CHAIN - 1] = -1;
    p->free_head = 0;
    pthread_mutex_init(&p->mtx, NULL);
}

void jobsys_chain_pool_free(jobsys_chain_pool *p) {
    pthread_mutex_destroy(&p->mtx);
}

int jobsys_chain_store(jobsys_chain_pool *p, const jobsys_job *job) {
    pthread_mutex_lock(&p->mtx);
    int idx = p->free_head;
    if (idx < 0) {
        pthread_mutex_unlock(&p->mtx);
        return -1;   // pool exhausted, caller falls back to direct submit
    }
    p->free_head = p->free_next[idx];
    p->slots[idx].job    = *job;
    p->slots[idx].in_use = 1;
    pthread_mutex_unlock(&p->mtx);
    return idx;
}

int jobsys_chain_take(jobsys_chain_pool *p, int idx, jobsys_job *out) {
    if (idx < 0 || idx >= JOBSYS_MAX_CHAIN) return 0;
    pthread_mutex_lock(&p->mtx);
    jobsys_chain_slot *s = &p->slots[idx];
    if (!s->in_use) {
        // already taken -- a continuation fired twice, swallow it.
        pthread_mutex_unlock(&p->mtx);
        return 0;
    }
    *out = s->job;
    s->in_use = 0;
    // push back onto the freelist
    p->free_next[idx] = p->free_head;
    p->free_head = idx;
    pthread_mutex_unlock(&p->mtx);
    return 1;
}
