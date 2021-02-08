#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 프로세스 세부 정보
struct process {
	char name;
	int at, bt, ct, ft, tt;
	int completed;
	float ntt;
} p[10];

int n;

// 도착 시간별로 프로세스 정렬
void sortByArrival()
{
	struct process temp;
	int i, j;

	// Selection Sort applied
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {

			// 적은 도착시간 확인
			if (p[i].at > p[j].at) {

				// 이전 프로세스를 앞쪽으로 바꿈
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
		puts("파일 오픈 실패");
		return ;
	}
	for (i = 0; i < n; i++) {
		fscanf(fp, "%d,%d,%d", &name[i], &arriv[i], &burst[i]);
	}
	fclose(fp);
	

	// 구조 변수 초기화
	for (i = 0, c = '1'; i < n; i++, c++) {
		p[i].name = c;
		p[i].at = arriv[i];
		p[i].bt = burst[i];

		// 완료 상태 변수
		// 대기중인 = 0
		// 완료 됨 = 1
		p[i].completed = 0;

		// 모든 버스트 시간의 합계에 대한 변수
		sum_bt += p[i].bt;
	}

	// 도착 시간별 구조 정렬
	sortByArrival();
	printf("\n프로세스ID\t도착시간\t서비스 시간(q)\t종료 시간");
	printf("\t반환 시간\t정규화된 반환 시간\t응답비율");
	for (t = p[0].at; t < sum_bt;) {

		// 응답 비율의 하한선 설정
		float hrr = -9999;

		// 응답 비율 변수
		float temp;

		// 선택한 다음 공정을 저장할 변수
		int loc;
		for (i = 0; i < n; i++) {

			// 프로세스가 도착했는지 불완전한지 확인
			if (p[i].at <= t && p[i].completed != 1) {

				// 응답 비율 계산
				temp = (p[i].bt + (t - p[i].at)) / p[i].bt;

				// 가장 높은 응답 비율 확인
				if (hrr < temp) {

					// 응답 비율 저장
					hrr = temp;

					// 위치 저장
					loc = i;
				}
			}
		}

		// 시간 값 업데이트
		t += p[loc].bt;

		// 종료 시간 계산
		p[loc].ft = t;

		// 회전 시간 계산
		p[loc].tt = t - p[loc].at;

		//합계 평균 시간
		avgtt += p[loc].tt;

		// 정규화 된 회전 시간 계산
		p[loc].ntt = ((float)p[loc].tt / p[loc].bt);

		// 완료 상태 업데이트
		p[loc].completed = 1;
		

		// 평균 대기 시간
		avgwt += p[loc].ft;
		printf("\n%c\t\t%d\t\t", p[loc].name, p[loc].at);
		printf("%d\t\t%d\t\t", p[loc].bt, p[loc].ft);
		printf("%d\t\t%f\t\t%.1lf\n", p[loc].tt, p[loc].ntt, p[loc].ntt);
	}
}
