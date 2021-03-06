#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <vector>

#include <bits/stdc++.h>

#pragma region input

static void input_clean_stdin (void) {

	int c = 0;
	do {
		c = getchar ();
	} while (c != '\n' && c != EOF);

}

// returns a newly allocated c string
static char *input_get_line (void) {

	size_t lenmax = 128, len = lenmax;
	char *line = (char *) malloc (lenmax), *linep = line;

	if (line) {
		int c = 0;

		for (;;) {
			c = fgetc (stdin);

			if (c == EOF || c == '\n') break;

			if (--len == 0) {
				len = lenmax;
				char * linen = (char *) realloc (linep, lenmax *= 2);

				if(linen == NULL) {
					free (linep);
					return NULL;
				}

				line = linen + (line - linep);
				linep = linen;
			}

			if ((*line++ = c) == '\n') break;
		}

		*line = '\0';
	}

	return linep;

}

#pragma endregion

#pragma region graph

static void path_print (int parent[], int s, int v);

static void graph_bfs (std::vector <int> adj[], int adjlen, int start, int end) {

	bool visited[adjlen];
	memset (visited, 0, sizeof (visited));
	std::queue <int> Q;
	visited[adjlen] = 1;
	Q.push (start);
	int parent[adjlen];
	memset (parent, -1, sizeof (parent));
	parent[start] = start;

	while (!Q.empty ()) {
		int u = Q.front ();
		// printf ("%d ", u);
		Q.pop ();

		std::vector<int>::iterator it;
		for (it = adj[u].begin (); it != adj[u].end (); it++) {
			if (!visited[*it]) {
				visited[*it] = 1;
				parent[*it] = u;
				Q.push (*it);
			}
		}
	}

	// printf("\nPaths\n");
	// for (int i = 0; i < v; i++) {
	// 	printf ("Path from %d to %d - ", s, i);
	// 	path_print (parent, s, i);
	// 	printf ("\n");
	// }

	bool found = false;
	for (unsigned int i = 0; i < adjlen; i++) {
		if (parent[i] == end) {
			found = true;
			break;
		}
	}

	if (found) {
		printf ("\n\nPath from %d to %d - ", start, end);
		path_print (parent, start, end);
		printf ("\n\n");
	}

	else {
		printf ("\nNo existe un camino de %d a %d\n", start, end);
	}

}

static void graph_add_edge (std::vector <int> adj[], int src, int dest) {

	adj[src].push_back (dest);
	// adj[dest].push_back (src);

}

static void path_print (int parent[], int s, int v) {

	if (s == v) {
		printf ("%d ", s);
		return;
	}

	path_print (parent, s, parent[v]);
	printf ("%d ", v);

}

#pragma endregion

#pragma region load

static unsigned int file_get_line (FILE *file, char *line, size_t line_size) {

	unsigned int retval = 1;

	if (!feof (file)) {
		if (fgets (line, line_size, file)) {
			size_t curr = strlen (line);

			if (line[curr - 1] == '\n') line[curr - 1] = '\0';

			retval = 0;
		}
	}

	return retval;

}

static unsigned int load_graph (
	std::vector <int> adj[],
	int *min, int *max,
	const char *filename
) {

	unsigned int retval = 1;

	if (filename) {
		FILE *file = fopen (filename, "r");
		if (file) {
			int value = 0;
			int one = 0, two = 0;
			char line[1024] = { 0 };
			char *token = NULL;
			bool first = true;
			while (!file_get_line (file, line, 1024)) {
				// printf ("%s\n", line);

				// we expect only 2 values
				first = true;
				token = strtok (line, "\t");
				while (token) {
					if (sscanf (token, "%d", &value) > 0) {
						printf ("%4d", value);

						if (value < *min) *min = value;
						if (value > *max) *max = value;

						if (first) {
							one = value;
							first = false;
						}

						else {
							two = value;
						}
					}

					token = strtok (NULL,"\t");
				}

				graph_add_edge (adj, one, two);

				printf ("\n");
			}

			retval = 0;

			fclose (file);
		}

		else {
			printf ("\nError al abrir el archivo: %s\n", filename);
		}
	}

	return retval;

}

#pragma endregion

static int get_start (int min, int max) {

	int start = 0;

	while (1) {
		printf ("\nIngresa el nodo inicial (%d - %d): ", min, max);
		scanf ("%d", &start);

		if ((start < min) || (start > max)) {
			printf ("\nEl valor de inicio %d es invalido, intenta de nuevo\n", start);
		}

		else {
			break;
		}
	}

	return start;

}

static int get_end (int min, int max) {

	int end = 0;

	while (1) {
		printf ("Ingresa el nodo destino (%d - %d): ", min, max);
		scanf ("%d", &end);

		if ((end < min) || (end > max)) {
			printf ("\nEl valor de destino %d es invalido, intenta de nuevo\n", end);
		}

		else {
			break;
		}
	}

	return end;

}

int main (int argc, char const **argv) {

	int adjlen = 32;
	std::vector <int> adj[adjlen];

	int min = 16384, max = 0;

	printf ("\nPractica 1 - Grafos\n");

	input_clean_stdin ();
	printf ("Ingresa el nombre del archivo con informacion del grafo: ");
	char *filename = input_get_line ();
	if (filename) {
		if (!load_graph (
			adj,
			&min, &max,
			filename
		)) {
			while (1) {
				int start = get_start (min, max);

				printf ("\n");

				int end = get_end (min, max);

				graph_bfs (adj, adjlen, start, end);
			}
		}

		free (filename);
	}

	return 0;

}