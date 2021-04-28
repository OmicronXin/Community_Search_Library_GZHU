#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<map>
#include<time.h>
#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<ctime>

using namespace std;

int main(int argc, char *argv[])
{
	//char feages[] = "C:\\RESEARCH\\Experiments\\GeoSocialCommunityDetection\\Datasets\\Gowalla\\Gowalla_edges.txt";
	//char fcheckins[] = "C:\\RESEARCH\\Experiments\\GeoSocialCommunityDetection\\Datasets\\Gowalla\\Gowalla_totalCheckins.txt";
	//char fcheckins[] = "E:\\Experiments\\GeoSocialCommunityDetection\\Datasets\\Test\\1.txt";
	//char fparteage[] = "C:\\RESEARCH\\Experiments\\GeoSocialCommunityDetection\\Datasets\\Gowalla\\Gowalla_part0\\Gowalla_edges_part0.txt";
	//char fpartcheckins[] = "C:\\RESEARCH\\Experiments\\GeoSocialCommunityDetection\\Datasets\\Gowalla\\Gowalla_part0\\Gowalla_totalCheckins_part0.txt";
	long toThisYear, numYearActive, minimumCommonPaperNumber;
	scanf("%ld %ld %ld", &toThisYear, &numYearActive, &minimumCommonPaperNumber);

	char cYear[10], cNumYear[10], cMinNum[10];
	sprintf(cYear, "%ld", toThisYear);
	sprintf(cNumYear, "%ld", numYearActive);
	sprintf(cMinNum, "%ld", minimumCommonPaperNumber);

	string s_cYear(cYear), s_cNumYear(cNumYear), s_cMinNum(cMinNum);
/***
https://dblp.org/xml/release/    	dblp-2020-12-01.xml.gz	2020-12-01 22:39	572M
Gephi£¬ nodeXL
***/

	string fdblp = "dblp-2020-12-01.xml";
	string fauthors = "dblp_authors_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";
	string fpapers = "dblp_papers_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";
	string faupaper = "dblp_au_paper_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";
	string faurelation = "dblp_au_relation_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";
	string fauthorsactive = "dblp_authors_active_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";
	string faupapernum = "dblp_au_papernum_" + s_cYear + "_" + s_cNumYear + "_" + s_cMinNum + ".txt";

	//char fdblp[] = "C:\\RESEARCH\\Datacentre\\dblp\\dblp.xml";
	//char fauthors[] = "C:\\RESEARCH\\Datacentre\\dblp\\dblp_authors0.txt";
	//char fpapers[] = "C:\\RESEARCH\\Datacentre\\dblp\\dblp_papers0.txt";
	//char faupaper[] = "C:\\RESEARCH\\Datacentre\\dblp\\dblp_au_paper0.txt";
	//char faurelation[] = "C:\\RESEARCH\\Datacentre\\dblp\\dblp_au_relation0.txt";

	FILE *fd = NULL;
	FILE *fa = NULL;
	FILE *fp = NULL;
	FILE *fap = NULL;
	FILE *far = NULL;
	FILE *faa = NULL;
	FILE *fpn = NULL;

	fd = fopen(fdblp.c_str(), "r");
	fa = fopen(fauthors.c_str(), "w");
	fp = fopen(fpapers.c_str(), "w");
	fap = fopen(faupaper.c_str(), "w");
	far = fopen(faurelation.c_str(), "w");
	faa = fopen(fauthorsactive.c_str(), "w");
	fpn = fopen(faupapernum.c_str(), "w");

	long count = 0;

	if (fd == NULL || fa == NULL || fp == NULL || fap == NULL || far == NULL)
	{
		printf("ERROR!");
		exit(1);
	}
	else
	{
		/*
		<dblp>
		<article mdate="2011-01-11" key="journals/acta/Saxena96">
		<author>Sanjeev Saxena</author>
		<title>Parallel Integer Sorting and Simulation Amongst CRCW Models.</title>
		<pages>607-619</pages>
		<year>1996</year>
		<volume>33</volume>
		<journal>Acta Inf.</journal>
		<number>7</number>
		<url>db/journals/acta/acta33.html#Saxena96</url>
		<ee>http://dx.doi.org/10.1007/BF03036466</ee>
		</article>
		<article mdate="2011-01-11" key="journals/acta/Simon83">
		<author>Hans-Ulrich Simon</author>
		<title>Pattern Matching in Trees and Nets.</title>
		<pages>227-248</pages>
		<year>1983</year>
		<volume>20</volume>
		<journal>Acta Inf.</journal>
		<url>db/journals/acta/acta20.html#Simon83</url>
		<ee>http://dx.doi.org/10.1007/BF01257084</ee>
		</article>
		*/
		map<vector<char>, long> authorsMap;
		map<vector<char>, long> papersMap;
		vector<map<long, long> > authorRelationMap;
		vector<vector<char> > authors;
		vector<vector<char> > papers;
		vector<vector<long> > author_paper;
		vector<vector<long> > author_relation, author_relation_num, authorsCount;
		vector<long> authorsActive, aupaperNum;

		map<long, long> emptyMap;
		vector<char> curString, auName, paperName, paperYear;
		vector<long> authorsList, empty, empty1;
		vector<char> end, booktitle, journal, author, year;
		char ch;
		end.push_back('/'); end.push_back('d'); end.push_back('b'); end.push_back('l'); end.push_back('p');
		booktitle.push_back('b'); booktitle.push_back('o'); booktitle.push_back('o'); booktitle.push_back('k'); booktitle.push_back('t'); booktitle.push_back('i'); booktitle.push_back('t'); booktitle.push_back('l'); booktitle.push_back('e');
		journal.push_back('j'); journal.push_back('o'); journal.push_back('u'); journal.push_back('r'); journal.push_back('n'); journal.push_back('a'); journal.push_back('l');
		author.push_back('a'); author.push_back('u'); author.push_back('t'); author.push_back('h'); author.push_back('o'); author.push_back('r');
		year.push_back('y'); year.push_back('e'); year.push_back('a'); year.push_back('r');

		curString.clear();
		long authorsMapID = 0;
		long papersMapID = 0;
		map<vector<char>, long>::iterator iter;
		map<long, long>::iterator iter1;

		for (long i = 0; i < numYearActive; i++)
		{
			empty1.push_back(0);
		}

		while (curString != end) //unique end tag
		{
			while (curString != author)
			{
				curString.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					ch = getc(fd);
				}
				ch = getc(fd);
				while (ch != '>')
				{
					curString.push_back(ch);
					ch = getc(fd);
				}
				if (curString == end)
				{
					break;
				}
			}
			if (curString == end)
			{
				break;
			}
			authorsList.clear();
			while (curString == author)
			{
				auName.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					auName.push_back(ch);
					ch = getc(fd);
				}
				bool authorExist = 0;
				iter = authorsMap.find(auName);
				if (iter == authorsMap.end())
				{
					authorsList.push_back(authorsMapID);
					authorsMap.insert(pair<vector<char>, long>(auName, authorsMapID));
					authorsMapID++;
					authors.push_back(auName);
					authorsCount.push_back(empty1);
					aupaperNum.push_back(0);
					author_paper.push_back(empty);
					author_relation.push_back(empty);
					author_relation_num.push_back(empty);
					authorRelationMap.push_back(emptyMap);
				}
				else
				{
					authorsList.push_back(iter->second);
				}
				ch = getc(fd);
				while (ch != '<')
				{
					ch = getc(fd);
				}
				ch = getc(fd);
				curString.clear();
				while (ch != '>')
				{
					curString.push_back(ch);
					ch = getc(fd);
				}
			}
			while (curString != year && curString != booktitle && curString != journal && curString != author)
			{
				curString.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					ch = getc(fd);
				}
				ch = getc(fd);
				while (ch != '>')
				{
					curString.push_back(ch);
					ch = getc(fd);
				}
				if (curString == end)
				{
					break;
				}
			}
			if (curString == end)
			{
				break;
			}
			long curYear = 9999;
			if (curString == year)
			{
				paperYear.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					paperYear.push_back(ch);
					ch = getc(fd);
				}
				curYear = (paperYear[0] - '0') * 1000 + (paperYear[1] - '0') * 100 + (paperYear[2] - '0') * 10 + paperYear[3] - '0';
			}
			if (curYear > toThisYear || curYear <= toThisYear - numYearActive)
			{
				//continue;
			}
			long yearTag = numYearActive - 1 - (toThisYear - curYear);
			if (authorsList.size() > 1)
			{
				for (long i = 0; i < authorsList.size(); i++)
				{
					long auID = authorsList[i];
					for (long j = 0; j < authorsList.size(); j++)
					{
						long auID1 = authorsList[j];
						if (auID1 != auID)
						{
							iter1 = authorRelationMap[auID].find(auID1);
							if (iter1 == authorRelationMap[auID].end())
							{
								author_relation[auID].push_back(auID1);
								author_relation_num[auID].push_back(1);
								authorRelationMap[auID].insert(pair<long, long>(auID1, authorRelationMap[auID].size()));
							}
							else
							{
								author_relation_num[auID][iter1->second]++;
							}

						}
					}
					if (yearTag > -1 && yearTag < numYearActive)
					{
						authorsCount[auID][yearTag] = 1;
						aupaperNum[auID]++;
					}
				}
			}
			while (curString != booktitle && curString != journal && curString != author)
			{
				curString.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					ch = getc(fd);
				}
				ch = getc(fd);
				while (ch != '>')
				{
					curString.push_back(ch);
					ch = getc(fd);
				}
				if (curString == end)
				{
					break;
				}
			}
			if (curString == end)
			{
				break;
			}
			if (curString != author)
			{
				paperName.clear();
				ch = getc(fd);
				while (ch != '<')
				{
					paperName.push_back(ch);
					ch = getc(fd);
				}
				long paperID;
				iter = papersMap.find(paperName);
				if (iter == papersMap.end())
				{
					paperID = papersMapID;
					papers.push_back(paperName);
					papersMap.insert(pair<vector<char>, long>(paperName, papersMapID));
					papersMapID++;
				}
				else
				{
					paperID = iter->second;
				}
				for (long i = 0; i < authorsList.size(); i++)
				{
					author_paper[authorsList[i]].push_back(paperID); //record every time of occurrence
				}
			}
			count++;
			if (count >= 10000 && count % 10000 == 0)
			{
				printf("new %ld processed\n", count);
			}
		}
		vector<char> writeTemp;
		//write to authors.txt
		for (long i = 0; i < authors.size(); i++)
		{
			writeTemp.clear();
			long authorID = i;
			while (1)
			{
				char c = char(authorID % 10 + '0');
				authorID /= 10;
				writeTemp.insert(writeTemp.begin(), c);
				if (authorID == 0)
				{
					break;
				}
			}
			for (long j = 0; j < writeTemp.size(); j++)
			{
				ch = putc(writeTemp[j], fa);
			}
			ch = putc('\t', fa);
			for (long j = 0; j < authors[i].size(); j++)
			{
				ch = putc(authors[i][j], fa);
			}
			ch = putc('\n', fa);
		}
		//write to papers.txt
		for (long i = 0; i < papers.size(); i++)
		{
			writeTemp.clear();
			long paperID = i;
			char c = (char)paperID % 10;
			while (1)
			{
				char c = char(paperID % 10 + '0');
				paperID /= 10;
				writeTemp.insert(writeTemp.begin(), c);
				if (paperID == 0)
				{
					break;
				}
			}
			for (long j = 0; j < writeTemp.size(); j++)
			{
				ch = putc(writeTemp[j], fp);
			}
			ch = putc('\t', fp);
			for (long j = 0; j < papers[i].size(); j++)
			{
				ch = putc(papers[i][j], fp);
			}
			ch = putc('\n', fp);
		}
		//write to author_paper.txt
		for (long i = 0; i < author_paper.size(); i++)
		{
			writeTemp.clear();
			long authorID = i;
			while (1)
			{
				char c = char(authorID % 10 + '0');
				authorID /= 10;
				writeTemp.insert(writeTemp.begin(), c);
				if (authorID == 0)
				{
					break;
				}
			}
			for (long j = 0; j < writeTemp.size(); j++)
			{
				ch = putc(writeTemp[j], fap);
			}
			for (long j = 0; j < author_paper[i].size(); j++)
			{
				long paperID = author_paper[i][j];
				writeTemp.clear();
				while (1)
				{
					char c = char(paperID % 10 + '0');
					paperID /= 10;
					writeTemp.insert(writeTemp.begin(), c);
					if (paperID == 0)
					{
						break;
					}
				}
				ch = putc('\t', fap);
				for (long k = 0; k < writeTemp.size(); k++)
				{
					ch = putc(writeTemp[k], fap);
				}
			}
			ch = putc('\n', fap);
		}
		//write to author_relation.txt
		for (long i = 0; i < author_relation.size(); i++)
		{
			for (long j = 0; j < author_relation[i].size(); j++)
			{
				if (author_relation_num[i][j] >= minimumCommonPaperNumber)
				{
					writeTemp.clear();
					long authorID = i;
					while (1)
					{
						char c = char(authorID % 10 + '0');
						authorID /= 10;
						writeTemp.insert(writeTemp.begin(), c);
						if (authorID == 0)
						{
							break;
						}
					}
					for (long k = 0; k < writeTemp.size(); k++)
					{
						ch = putc(writeTemp[k], far);
					}
					ch = putc('\t', far);
					writeTemp.clear();
					long paperID = author_relation[i][j];
					while (1)
					{
						char c = char(paperID % 10 + '0');
						paperID /= 10;
						writeTemp.insert(writeTemp.begin(), c);
						if (paperID == 0)
						{
							break;
						}
					}
					for (long k = 0; k < writeTemp.size(); k++)
					{
						ch = putc(writeTemp[k], far);
					}
					ch = putc('\n', far);
				}
			}
		}
		char record[100];
		//write to authors_active.txt
		for (long i = 0; i < authors.size(); i++)
		{
			writeTemp.clear();
			long count = 0;
			for (long j = 0; j < authorsCount[i].size(); j++)
			{
				if (authorsCount[i][j])
				{
					count++;
				}
			}
			if (count > numYearActive - 1)
			{
				sprintf(record, "%ld", i);
				fwrite(record, sizeof(*record), strlen(record), faa);
				ch = putc('\n', faa);
			}
		}
		//write to au_papernum.txt
		for (long i = 0; i < authors.size(); i++)
		{
			writeTemp.clear();
			sprintf(record, "%ld", aupaperNum[i]);
			fwrite(record, sizeof(*record), strlen(record), fpn);
			ch = putc('\n', fpn);
		}
	}

	fclose(fd);
	fclose(fa);
	fclose(fp);
	fclose(fap);
	fclose(far);
	fclose(faa);
	fclose(fpn);
	return 0;
}
