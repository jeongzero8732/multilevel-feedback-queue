#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���μ��� ���� ����
struct process {
	char name;
	int at, bt, ct, ft, tt;
	int completed;
	float ntt;
} p[10];

int n;

// ���� �ð����� ���μ��� ����
void sortByArrival()
{
	struct process temp;
	int i, j;

	// Selection Sort applied
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {

			// ���� �����ð� Ȯ��
			if (p[i].at > p[j].at) {

				// ���� ���μ����� �������� �ٲ�
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
}

void main()
{
	FILE *fp;
	int arriv[5];
	int burst[5];
	int name[5];
	double R;
	char* inp;
	int i, j, t, sum_bt = 0;
	char c;
	float avgwt = 0, avgtt = 0;
	n = 5;
	fp = fopen("input.txt", "rt");
	if (fp == NULL) {
		puts("���� ���� ����");
		return ;
	}
	for (i = 0; i < n; i++) {
		fscanf(fp, "%d,%d,%d", &name[i], &arriv[i], &burst[i]);
	}
	fclose(fp);
	

	// ���� ���� �ʱ�ȭ
	for (i = 0, c = '1'; i < n; i++, c++) {
		p[i].name = c;
		p[i].at = arriv[i];
		p[i].bt = burst[i];

		// �Ϸ� ���� ����
		// ������� = 0
		// �Ϸ� �� = 1
		p[i].completed = 0;

		// ��� ����Ʈ �ð��� �հ迡 ���� ����
		sum_bt += p[i].bt;
	}

	// ���� �ð��� ���� ����
	sortByArrival();
	printf("\n���μ���ID\t�����ð�\t���� �ð�(q)\t���� �ð�");
	printf("\t��ȯ �ð�\t����ȭ�� ��ȯ �ð�\t�������");
	for (t = p[0].at; t < sum_bt;) {

		// ���� ������ ���Ѽ� ����
		float hrr = -9999;

		// ���� ���� ����
		float temp;

		// ������ ���� ������ ������ ����
		int loc;
		for (i = 0; i < n; i++) {

			// ���μ����� �����ߴ��� �ҿ������� Ȯ��
			if (p[i].at <= t && p[i].completed != 1) {

				// ���� ���� ���
				temp = (p[i].bt + (t - p[i].at)) / p[i].bt;

				// ���� ���� ���� ���� Ȯ��
				if (hrr < temp) {

					// ���� ���� ����
					hrr = temp;

					// ��ġ ����
					loc = i;
				}
			}
		}

		// �ð� �� ������Ʈ
		t += p[loc].bt;

		// ���� �ð� ���
		p[loc].ft = t;

		// ȸ�� �ð� ���
		p[loc].tt = t - p[loc].at;

		//�հ� ��� �ð�
		avgtt += p[loc].tt;

		// ����ȭ �� ȸ�� �ð� ���
		p[loc].ntt = ((float)p[loc].tt / p[loc].bt);

		// �Ϸ� ���� ������Ʈ
		p[loc].completed = 1;
		

		// ��� ��� �ð�
		avgwt += p[loc].ft;
		printf("\n%c\t\t%d\t\t", p[loc].name, p[loc].at);
		printf("%d\t\t%d\t\t", p[loc].bt, p[loc].ft);
		printf("%d\t\t%f\t\t%.1lf\n", p[loc].tt, p[loc].ntt, p[loc].ntt);
	}
}
