#pragma once

#include <assert.h>
#include <iosfwd>
#include <vector>
#include <string>


class IStrArgSeg
{
public:
	virtual bool	empty(void) const =0;
	virtual size_t	size(void)  const =0;
	virtual const IStrArgSeg& operator [] ( size_t index ) const =0;
	virtual std::string	getStr(void) const =0;
	operator std::string(void) const
	{
		return getStr();
	}
protected:
	IStrArgSeg() {}
	IStrArgSeg( const IStrArgSeg& /*other*/ ) {}
	virtual ~IStrArgSeg() {}
	IStrArgSeg& operator = ( const IStrArgSeg& /*other*/ ) { return *this; }
};

// example:
// 	pattern:��`;,��
// 	args:��1,2;3,4`5,6;7,8`��
class StrArgPkg : public IStrArgSeg
{
public:
	StrArgPkg( const std::string& pattern ="", const std::string& args ="" )
		: m_pattern( pattern ), m_args( args ), m_rootSeg( m_args )
	{
		parse();
	}
	~StrArgPkg()
	{
	}
public:
	void	setPattern( const std::string& pattern )
	{
		m_pattern= pattern;
		parse();
	}
	const std::string&	getPattern(void) const
	{
		return m_pattern;
	}
	void	setArgs( const std::string& args )
	{
		m_args= args;
		parse();
	}
	const std::string&	getArgs(void) const
	{
		return m_args;
	}
	bool	empty(void) const
	{
		return m_rootSeg.empty();
	}
	size_t	size(void) const
	{
		return m_rootSeg.size();
	}
	const IStrArgSeg& operator [] ( size_t index ) const
	{
		return m_rootSeg[index];
	}
	std::string	getStr(void) const
	{
		return m_args;
	}
private:
	class StrArgSeg : public IStrArgSeg
	{
	public:
		typedef std::vector< StrArgSeg >	StrArgSegs;
	public:
		StrArgSeg( const std::string& args )
			: m_args( &args )
		{
			clear();
		}
		~StrArgSeg()
		{
		}
	public:
		bool	empty(void) const
		{
			return m_segs.empty();
		}
		size_t	size(void) const
		{
			return m_segs.size();
		}
		const IStrArgSeg& operator [] ( size_t index ) const
		{
			assert( index < m_segs.size() );
			return m_segs[index];
		}
		std::string	getStr(void) const
		{
			assert( m_args );
			assert( m_begin < m_args->size() );
			assert( m_begin + m_len <= m_args->size() );
			assert( m_len >= 1 );
			return m_args->substr( m_begin, m_len - 1 );
		}
		void	clear(void)
		{
			m_begin= 0;
			m_len= 0;
			m_segs.clear();
		}
		void	pushSeg( size_t totalLevel, size_t newLevel, size_t endPos )
		{
			assert( m_args );
			assert( endPos < m_args->size() );
			StrArgSeg* parent = this;
			for(size_t parentLevel = 0; parentLevel < newLevel; ++parentLevel)
			{
				assert( parent );
				assert( ! parent->m_segs.empty() );
				parent = & parent->m_segs.back();
				assert( parent );
				assert( endPos >= parent->m_begin + parent->m_len );
				parent->m_len = 1 + endPos - parent->m_begin;
			}
			assert( parent );
			size_t newBegin= 0;
			if( ! parent->m_segs.empty() )
			{
				StrArgSeg& prevSeg= parent->m_segs.back();
				newBegin= prevSeg.m_begin + prevSeg.m_len;
			}
			size_t newLen= 1 + endPos - newBegin;
			for( size_t i = newLevel; i < totalLevel; ++i )
			{
				assert( parent );
				parent->m_segs.push_back( StrArgSeg( *m_args ) );
				StrArgSeg& newSeg= parent->m_segs.back();
				newSeg.m_begin= newBegin;
				newSeg.m_len= newLen;
				parent = &newSeg;
			}
		}
	private:
		const std::string*	m_args;
		size_t				m_begin;
		size_t				m_len;
		StrArgSegs			m_segs;
	};
	typedef StrArgSeg::StrArgSegs	StrArgSegs;
private:
	void	parse(void)
	{
		using std::string;

		m_rootSeg.clear();
		if( m_pattern.empty() )
		{
			return;
		}

		if( m_args.empty() )
		{
			return;
		}

		if( m_pattern.rfind( m_args[ m_args.size()-1 ] ) == string::npos )
		{
			m_args += m_pattern[0];
		}

		size_t finishedLevel= 0;
		for( size_t i= 0; i<m_args.size(); ++i )
		{
			char now= m_args[i];
			string::size_type level= m_pattern.rfind( now );

			if( i == m_args.size() - 1 )
			{
				assert( level != string::npos );
			}

			if( level == string::npos )
			{
				continue;
			}

			m_rootSeg.pushSeg( m_pattern.size(), finishedLevel, i );
			finishedLevel = level;
		}
	}
private:
	std::string	m_pattern;
	std::string	m_args;
	StrArgSeg	m_rootSeg;
};


inline std::string getArg( const IStrArgSeg& seg, size_t index, const std::string& defaultValue ="" )
{
	if( index >= seg.size() )
	{
		return defaultValue;
	}
	return seg[index].getStr();
}
