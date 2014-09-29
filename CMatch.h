#ifndef CMATCH_H
#define CMATCH_H
class CMatch{
	static int match(string s,int pt,string md){
		int m=md.size();
		int n=s.size()-m+1;
		for (int i=pt;i<n;i++)
		{
			bool x=true;
			for (int j=i,k=0;k<m;k++,j++)
			{
				if (md.at(k)!='?'&&md.at(k)!=s.at(j))
				{
					x=false;
					break;
				}
			}
			if (x)
			{
				return i;
			}
		}
		return -1;
	}
public:
	static bool judge(string s,string smod){
		if (smod=="")
		{
			smod="*";
		}
		int ltmp=0;
		int n=smod.size();
		vector<string> mods;
		for (int i=0;i<n;i++)
		{
			if (smod.at(i)=='*')
			{
				string m2="";
				for (int j=ltmp,k=0;j<i;k++,j++)
				{
					m2+=smod.at(j);
				}
				mods.push_back(m2);
				ltmp=i+1;
			}
		}
		string m2="";
		for (int j=ltmp,k=0;j<n;k++,j++)
		{
			m2+=smod.at(j);
		}
		mods.push_back(m2);
		n=s.size();
		int m=mods[0].size();
		for (int i=0;i<m;i++)
		{
			char c=mods[0].at(i);
			if (c!='?'&&c!=s.at(i))
			{
				return false;
			}
		}
		if (mods.size()==1)
		{
			return true;
		}
		s=s.substr(m);
		n-=m;
		int p=mods.size()-1;
		m=mods[p].size();
		for (int i=0;i<m;i++)
		{
			char c=mods[p].at(i);
			if (c!='?'&&c!=s.at(n-m+i))
			{
				return false;
			}
		}
		n-=m;
		int pt=0;
		for (int i=1;i<p;i++)
		{
			if (mods[i].size()>0)
			{
				int pp=match(s,pt,mods[i]);
				if (pp==-1)
				{
					return false;
				}
				pt=pp+mods[i].size();
			}
		}
		return true;
	}
};
#endif