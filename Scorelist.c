#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Scorelist.h"
#include "UI.h"

#define NAMELEN 20
#define SCORES 10

typedef struct Score {
	char name[NAMELEN + 1];
	int level;
	int score;
} Score;

static Score scores[SCORES];

void
Scorelist_read() {
	FILE *scorefile = fopen(SCOREFILE, "r");
	int i;

	if (scorefile != NULL) {
		for (i = 0; i < SCORES; i++)
			fscanf(scorefile, "%20s%d%d\n", scores[i].name,
			       &scores[i].level, &scores[i].score);
		fclose(scorefile);
	}
	else {
		for (i = 0; i < SCORES; i++) {
			strcpy(scores[i].name, "Anonymous");
			scores[i].level = 0;
			scores[i].score = 0;
		}
	}
}

void
Scorelist_write() {
	FILE *scorefile = fopen(SCOREFILE, "w");
	int i;
	if (scorefile == NULL)
		return;
	for (i = 0; i < SCORES; i++)
		fprintf(scorefile, "%-*s %d %d\n", NAMELEN,
			scores[i].name, scores[i].level, scores[i].score);
	fclose(scorefile);
}

/*  Add new high score to list   */
void
Scorelist_recalc(const char *str, int level, int score) {
	int i;
	char tname[NAMELEN + 1];
	char *nl;

	if (scores[SCORES - 1].score >= score)
		return;
	for (i = SCORES - 1; i > 0; i--) {
		if (scores[i - 1].score < score) {
			strcpy (scores[i].name, scores[i - 1].name);
			scores[i].level = scores[i - 1].level;
			scores[i].score = scores[i - 1].score;
		}
		else
			break;
	}

	memset(tname, 0, sizeof(tname));
	if (str == NULL || str[0] == 0)
		strcpy(tname, "Anonymous");
	strncpy(tname, str, sizeof(tname) - 1);
	nl = strchr(tname,'\n');
	if (nl != NULL)
		*nl = 0;
	
	strcpy(scores[i].name, tname);
	scores[i].level = level;
	scores[i].score = score;
}

void
Scorelist_update() {
	char str[500];
	int i;
	sprintf(str, "%s\n\n", "High Scores:");
	sprintf(str, "%s%-*s %6s %7s\n", str, NAMELEN,
		"Name", "Level", "Score");
	for (i = 0; i < SCORES; i++) {
		sprintf(str, "%s%-*s %6d %7d\n", str, NAMELEN,
			scores[i].name, scores[i].level, scores[i].score);
	}
	UI_update_dialog(DIALOG_HIGHSCORE, str);
}

int
Scorelist_ishighscore(int val) {
	return (val > scores[SCORES - 1].score);
}
