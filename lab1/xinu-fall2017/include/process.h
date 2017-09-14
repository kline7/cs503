/* process.h - isbadpid */

/* Maximum number of processes in the system */

#ifndef NPROC
#define	NPROC		8
#endif	

/* Define Block status                 */

#define BLOCKED      0
#define UNBLOCKED    1

/* Define tsnew status    */

#define TSFIRST      0
#define TSSECOND     1


/* Define the group of the the process*/
#define PROPORTIONALSHARE    0 /*Process group PS scheduling*/
#define TSSCHED              1 /*Process group TS scheduling*/

/* Process state constants */

#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/

/* Miscellaneous process definitions */

#define	PNMLEN		16	/* Length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* Process initialization constants */

#define	INITSTK		65536	/* Initial process stack size		*/
#define	INITPRIO	20	/* Initial process priority		*/
#define	INITRET		userret	/* Address to which process returns	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		5	/* must be odd to make procent 4N bytes	*/

/* Definition of the process table (multiple of 32 bits) */

struct procent {		/* Entry in the process table		*/
	uint16	prstate;	/* Process state: PR_CURR, etc.		*/
	int     prgroup;    /* Process group: PS or TS              */
	pri16	prprio;		/* Process priority			*/
	char	*prstkptr;	/* Saved stack pointer			*/
	char	*prstkbase;	/* Base of run time stack		*/
	uint32	prstklen;	/* Stack length in bytes		*/
	char	prname[PNMLEN];	/* Process name				*/
	sid32	prsem;		/* Semaphore on which process waits	*/
	pid32	prparent;	/* ID of the creating process		*/
	umsg32	prmsg;		/* Message sent to this process		*/
	bool8	prhasmsg;	/* Nonzero iff msg is valid		*/
	int16	prdesc[NDESC];	/* Device descriptors for process	*/

	/* this is for PS group*/
	
	pri16   pspi;         /* PS priority for process in PS group  */
	int     psrate;         /* PS rate for process in PS group      */
	int     psblock;        /* PS block status                      */

	/* this is for TS group*/
	
	int     tsnew;        /* TS is scheduled for first time or not*/
};


/* Definition of the TS update */

struct tsupdate{
	uint32  ts_quantum;   /* the updating quantum time  */
	pri16 	ts_tqexp;     /* the updating level for CPU */
	pri16   ts_slepret;   /* the updating level for I/O */

};

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern	struct	procent  proctab[];
extern  struct  tsupdate tstable[];
extern	int32	prcount;	/* Currently active processes		*/
extern	pid32	currpid;	/* Currently executing process		*/
extern  pri16   psprio;         /* Currently priotity of group PS       */
extern  pri16   tsprio;         /* Currently priotity of group TS       */
extern  pri16   psprioin;       /* Initial priotity of group PS         */
extern  pri16   tsprioin;       /* Initial priotity of group TS         */
