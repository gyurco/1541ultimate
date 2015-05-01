/*
 * file_info.h
 *
 *  Created on: Apr 27, 2015
 *      Author: Gideon
 */

#ifndef FILESYSTEM_FILE_INFO_H_
#define FILESYSTEM_FILE_INFO_H_

#include "integer.h"

/* File attribute bits for directory entry */

#define AM_RDO   0x01    /* Read only */
#define AM_HID   0x02    /* Hidden */
#define AM_SYS   0x04    /* System */
#define AM_VOL   0x08    /* Volume label */
#define AM_LFN   0x0F    /* LFN entry */
#define AM_DIR   0x10    /* Directory */
#define AM_ARC   0x20    /* Archive */
#define AM_MASK  0x3F    /* Mask of defined bits */
#define AM_HASCHILDREN 0x40 /* Is the kind of file that might have a directory listing */

class FileSystem;

class FileInfo
{
public:
	FileSystem *fs;  /* Reference to file system, to uniquely identify file */
    DWORD   cluster; /* Start cluster, easy for open! */
    DWORD	size;	 /* File size */
	WORD	date;	 /* Last modified date */
	WORD	time;	 /* Last modified time */
	DWORD   dir_clust;  /* Start of directory, needed to reopen dir */
    WORD    dir_index;  /* Entry of the directory we have our directory item. */
    WORD    lfsize;
    char   *lfname;
	BYTE	attrib;	 /* Attribute */
    char    extension[4];

    FileInfo()
    {
    	init();
    }

    void init()
    {
    	memset(this, 0, sizeof(FileInfo));
    }

    FileInfo(int namesize)
	{
    	init();
		lfsize = namesize;
	    if (namesize) {
	        lfname = new char[namesize];
			lfname[0] = '\0';
		}
    }

    FileInfo(char *name)
    {
    	init();
		lfsize = strlen(name)+1;
        lfname = new char[lfsize];
        strcpy(lfname, name);
    }

    FileInfo(FileInfo &i)
    {
    	fs = i.fs;
        cluster = i.cluster;
        size = i.size;
        date = i.date;
        time = i.time;
        dir_clust = i.dir_clust;
        dir_index = i.dir_index;
        lfsize = i.lfsize;
        lfname = new char[lfsize];
        strncpy(lfname, i.lfname, lfsize);
        strncpy(extension, i.extension, 4);
        attrib = i.attrib;
        extension[3] = 0;
    }

    FileInfo(FileInfo *i, char *new_name)
    {
        fs = i->fs;
        cluster = i->cluster;
        size = i->size;
        date = i->date;
        time = i->time;
        dir_clust = i->dir_clust;
        dir_index = i->dir_index;
		lfsize = strlen(new_name)+1;
        lfname = new char[lfsize];
        strcpy(lfname, new_name);
        attrib = i->attrib;
        extension[0] = 0;
    }

	~FileInfo()
	{
	    if(lfname)
	        delete lfname;
    }

	bool is_directory(void) {
	    return (attrib & AM_DIR);
	}

	bool is_writable(void);

	void print_info(void) {
		printf("File System: %p\n", fs);
		printf("Cluster    : %d\n", cluster);
		printf("Size       : %d\n", size);
		printf("Date       : %d\n", date);
		printf("Time	   : %d\n", time);
        printf("DIR index  : %d\n", dir_index);
		printf("LFSize     : %d\n", lfsize);
		printf("LFname     : %s\n", lfname);
		printf("Attrib:    : %b\n", attrib);
		printf("Extension  : %s\n", extension);
		printf("Dir Clust  : %d\n", dir_clust);
	}
};


#endif /* FILESYSTEM_FILE_INFO_H_ */