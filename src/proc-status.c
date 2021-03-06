 /*
 * Copyright (c) 2012, the ACDC Project Authors.
 * All rights reserved. Please see the AUTHORS file for details.
 * Use of this source code is governed by a BSD license that
 * can be found in the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proc-status.h"
#include "acdc.h"

#ifdef __linux__

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>

#ifndef LINE_MAX
#define LINE_MAX 200
#endif

struct proc_status {
        long vm_size;
        long vm_rss;
        long vm_data;
};

volatile struct proc_status stat;

__attribute__((unused))
static inline void fgets_nn(char *str, int size, FILE *stream) {
	char *s = str;
	int c;
	while (--size > 0 && (c = fgetc(stream)) != EOF)
		*s++ = c;
	*s = '\0';
}

static long get_long_from_line(char *str, int index) {
	char *token;
	int i = 0;
	const char *delimiter = " ";
	char *sp = str;
	while (*sp != '\0' && i < LINE_MAX) {
		if (*sp == '\t') *sp = ' ';
		++i;
		++sp;
	}
	
	for (sp = str, i = 0; ; sp = NULL) {
		token = strtok(sp, delimiter);
		if (token == NULL) break;
		if (i++ == index) return atol(token);
	}
	return 0;
}


size_t get_dirty_hugepages(pid_t pid) {

	char filename[50]; //more than enough for /proc/[pid]/numa_maps
	snprintf(filename, 50, "/proc/%d/numa_maps", pid);
	FILE *stream;
	char buf[LINE_MAX] = {0};
	size_t num_dirty = 0;

	stream = fopen(filename, "r");
	if (!stream) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
        while (fgets(buf, LINE_MAX, stream)) {
                char *r = strstr(buf, "anon_hugepage");
                if (r != NULL) {
	                char numbuf[LINE_MAX] = {0};
                        char *numbufp = numbuf;

                        r = strstr(buf, "dirty");
                        r+=6;

                        while (*r != ' ') {
                                *numbufp = *r;
                                numbufp++;
                                r++;
                        }
                        num_dirty += atol(numbuf);
                        //printf("And the number is: %lu\n", num_dirty);
                }
	}

	if (fclose(stream)) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}
        return num_dirty;
}

void update_proc_status(pid_t pid) {
	
	char filename[50]; //more than enough for /proc/[pid]/status
	snprintf(filename, 50, "/proc/%d/status", pid);
	FILE *stream;
	char buf[LINE_MAX] = {0};
	//size_t len;

	stream = fopen(filename, "r");
	if (!stream) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	//reset
	stat.vm_data = 0;
	stat.vm_rss = 0;
	stat.vm_size = 0;

	while (fgets(buf, LINE_MAX, stream)) {
		if (strncmp(buf, "VmSize", 6) == 0) {
			stat.vm_size = get_long_from_line(buf, 1);
		}
		if (strncmp(buf, "VmRSS", 5) == 0) {
			stat.vm_rss = get_long_from_line(buf, 1);
		}
		if (strncmp(buf, "VmData", 6) == 0) {
			stat.vm_data = get_long_from_line(buf, 1);
		}
	}

	if (fclose(stream)) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}

        //take huge pages into account
        size_t dirty_hugepages = get_dirty_hugepages(pid);
        //printf("Adding %lu hugepages to RSS\n", dirty_hugepages);
        stat.vm_rss += dirty_hugepages * HUGEPAGE_KB;
}


long get_vm_size() {
	return stat.vm_size;
}
long get_resident_set_size() {
	return stat.vm_rss;
}
long get_data_segment_size() {
	return stat.vm_data;
}

#endif // __linux__

#ifdef __APPLE__

#include <sys/sysctl.h>
#include <mach/mach.h>
#include <stdint.h>

uint64_t _rss;

void update_proc_status(pid_t pid) {

        struct task_basic_info ti;
        mach_msg_type_number_t count = TASK_BASIC_INFO_64_COUNT;

        kern_return_t kr = task_info(mach_task_self(), TASK_BASIC_INFO_64,
                                     (task_info_t) &ti, &count);

        if (kr != KERN_SUCCESS) {
                printf("task_info failed\n");
                exit(kr);
        }

        _rss = ti.resident_size / 1024;


}

size_t get_dirty_hugepages(pid_t pid) {
        return 0;
}

long get_vm_peak() {
        return 0;
}

long get_vm_size() {
        return 0;
}

long get_resident_set_size() {
        return _rss;
}

long get_high_water_mark() {
        return 0;
}

long get_data_segment_size() {
        return 0;
}


#endif // __APPLE__



