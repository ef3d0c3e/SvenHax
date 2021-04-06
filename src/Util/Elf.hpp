#ifndef UTIL_ELF_HPP
#define UTIL_ELF_HPP

#include <iostream>

#include "Cenum.hpp"
#include "Maps.hpp"
#include <string>
#include <functional>
#include <memory>
#include <link.h>
#include <cxxabi.h>
#include <deque>

using namespace std::literals;

MAKE_CENUM_Q(DemanglerType, std::uint8_t,
	NONE, 0,
	GNUCXXABI, 1,
);

template <DemanglerType Demangler = DemanglerType::NONE>
class ELF
{
	using Ehdr = ElfW(Ehdr);
	using Shdr = ElfW(Shdr);
	using Phdr = ElfW(Phdr);
	using Half = ElfW(Half);
	using Word = ElfW(Word);
	using Xword = ElfW(Xword);
	using Sword = ElfW(Sword);
	using Addr = ElfW(Addr);
	using Off = ElfW(Off);
	using ESection = ElfW(Section);
#if __x86_64__
	using Esym = Elf64_Sym;
#else
	using Esym = Elf32_Sym;
#endif
public:
	// {{{ Enums
	MAKE_CENUM_Q(ObjectType, Half,
		NONE, ET_NONE,
		REL, ET_REL,
		EXEC, ET_EXEC,
		DYN, ET_DYN,
		CORE, ET_CORE,
		NUM, ET_NUM,
		LOOS, ET_LOOS,
		HIOS, ET_HIOS,
		LOPROC, ET_LOPROC,
		HIPROC, ET_HIPROC,
	);

	MAKE_CENUM_Q(VersionType, Word,
		NONE, EV_NONE,
		CURRENT, EV_CURRENT,
		NUM, EV_NUM,
	);

	MAKE_CENUM_Q(SectionType, Word, // Shdr::sh_type
		NUL, SHT_NULL,
		PROGBITS, SHT_PROGBITS,
		SYMTAB, SHT_SYMTAB,
		STRTAB, SHT_STRTAB,
		RELA, SHT_RELA,
		HASH, SHT_HASH,
		DYNAMIC, SHT_DYNAMIC,
		NOTE, SHT_NOTE,
		NOBITS, SHT_NOBITS,
		REL, SHT_REL,
		SHLIB, SHT_SHLIB,
		DYNSYM, SHT_DYNSYM,
		INIT_ARRAY, SHT_INIT_ARRAY,
		FINI_ARRAY, SHT_FINI_ARRAY,
		PREINIT_ARRAY, SHT_PREINIT_ARRAY,
		GROUP, SHT_GROUP,
		SYMTAB_SHNDX, SHT_SYMTAB_SHNDX,
		NUM, SHT_NUM,
		LOOS, SHT_LOOS,
		GNU_ATTRIBUTES, SHT_GNU_ATTRIBUTES,
		GNU_HASH, SHT_GNU_HASH,
		GNU_LIBLIST, SHT_GNU_LIBLIST,
		CHECKSUM, SHT_CHECKSUM,
		LOSUNW, SHT_LOSUNW,
		SUNW_move, SHT_SUNW_move,
		SUNW_COMDAT, SHT_SUNW_COMDAT,
		SUNW_syminfo, SHT_SUNW_syminfo,
		GNU_verdef, SHT_GNU_verdef,
		GNU_verneed, SHT_GNU_verneed,
		GNU_versym, SHT_GNU_versym,
		HISUNW, SHT_HISUNW,
		HIOS, SHT_HIOS,
		LOPROC, SHT_LOPROC,
		HIPROC, SHT_HIPROC,
		LOUSER, SHT_LOUSER,
		HIUSER, SHT_HIUSER,
	);

	MAKE_CENUM_Q(SectionFlag, Word, // Shdr::sh_flags
		WRITE, SHF_WRITE,
		ALLOC, SHF_ALLOC,
		EXECINSTR, SHF_EXECINSTR,
		MERGE, SHF_MERGE,
		STRINGS, SHF_STRINGS,
		INFO_LINK, SHF_INFO_LINK,
		LINK_ORDER, SHF_LINK_ORDER,
		OS_NONCONFORMING, SHF_OS_NONCONFORMING,
		GROUP, SHF_GROUP,
		TLS, SHF_TLS,
		COMPRESSED, SHF_COMPRESSED,
		MASKOS, SHF_MASKOS,
		MASKPROC, SHF_MASKPROC,
		ORDERED, SHF_ORDERED,
		EXCLUDE, SHF_EXCLUDE,
	);

	MAKE_CENUM_Q(SegmentType, Word, // Phdr::p_type
		NUL, PT_NULL,
		LOAD, PT_LOAD,
		DYNAMIC, PT_DYNAMIC,
		INTERP, PT_INTERP,
		NOTE, PT_NOTE,
		SHLIB, PT_SHLIB,
		PHDR, PT_PHDR,
		TLS, PT_TLS,
		NUM, PT_NUM,
		LOOS, PT_LOOS,
		GNU_EH_FRAME, PT_GNU_EH_FRAME,
		GNU_STACK, PT_GNU_STACK,
		GNU_RELRO, PT_GNU_RELRO,
		LOSUNW, PT_LOSUNW,
		SUNWBSS, PT_SUNWBSS,
		SUNWSTACK, PT_SUNWSTACK,
		HISUNW, PT_HISUNW,
		HIOS, PT_HIOS,
		LOPROC, PT_LOPROC,
		HIPROC, PT_HIPROC,
	);

	MAKE_CENUM_Q(SegmentFlag, Word, // Phdr::p_flags
		X, PF_X,
		W, PF_W,
		R, PF_R,
		MASKOS, PF_MASKOS,
		MASKPROC, PF_MASKPROC,
	);

	MAKE_CENUM_Q(TagType, Sword,
		NUL, DT_NULL,
		NEEDED, DT_NEEDED,
		PLTRELSZ, DT_PLTRELSZ,
		PLTGOT, DT_PLTGOT,
		HASH, DT_HASH,
		STRTAB, DT_STRTAB,
		SYMTAB, DT_SYMTAB,
		RELA, DT_RELA,
		RELASZ, DT_RELASZ,
		RELAENT, DT_RELAENT,
		STRSZ, DT_STRSZ,
		SYMENT, DT_SYMENT,
		INIT, DT_INIT,
		FINI, DT_FINI,
		SONAME, DT_SONAME,
		RPATH, DT_RPATH,
		SYMBOLIC, DT_SYMBOLIC,
		REL, DT_REL,
		RELSZ, DT_RELSZ,
		RELENT, DT_RELENT,
		PLTREL, DT_PLTREL,
		DEBUG, DT_DEBUG,
		TEXTREL, DT_TEXTREL,
		JMPREL, DT_JMPREL,
		BIND_NOW, DT_BIND_NOW,
		INIT_ARRAY, DT_INIT_ARRAY,
		FINI_ARRAY, DT_FINI_ARRAY,
		INIT_ARRAYSZ, DT_INIT_ARRAYSZ,
		FINI_ARRAYSZ, DT_FINI_ARRAYSZ,
		RUNPATH, DT_RUNPATH,
		FLAGS, DT_FLAGS,
		ENCODING, DT_ENCODING,
		PREINIT_ARRAY, DT_PREINIT_ARRAY,
		PREINIT_ARRAYSZ, DT_PREINIT_ARRAYSZ,
		SYMTAB_SHNDX, DT_SYMTAB_SHNDX,
		NUM, DT_NUM,
		LOOS, DT_LOOS,
		HIOS, DT_HIOS,
		LOPROC, DT_LOPROC,
		HIPROC, DT_HIPROC,
		PROCNUM, DT_PROCNUM,
	);
	// }}}

	// Our enums
	MAKE_CENUM_Q(SymbolType, std::uint8_t,
		LOCAL, STB_LOCAL,
		GLOBAL, STB_GLOBAL,
		WEAK, STB_WEAK,
		NUM, STB_NUM,
		LOOS, STB_LOOS,
		GNU_UNIQUE, STB_GNU_UNIQUE,
		HIOS, STB_HIOS,
		LOPROC, STB_LOPROC,
		HIPROC, STB_HIPROC,
	);

	MAKE_CENUM_Q(SymbolBind, std::uint8_t,
		NOTYPE, STT_NOTYPE,
		OBJECT, STT_OBJECT,
		FUNC, STT_FUNC,
		SECTION, STT_SECTION,
		FILE, STT_FILE,
		COMMON, STT_COMMON,
		TLS, STT_TLS,
		LOOS, STT_LOOS,
		GNU_IFUNC, STT_GNU_IFUNC,
		HIOS, STT_HIOS,
		LOPROC, STT_LOPROC,
		HIPROC, STT_HIPROC,
	);

	MAKE_CENUM_Q(SymbolVisibility, std::uint8_t,
		DEFAULT, STV_DEFAULT,
		INTERNAL, STV_INTERNAL,
		HIDDEN, STV_HIDDEN,
		PROTECTED, STV_PROTECTED,
    );

	struct Section
	{
		Half index;

#if __x86_64__
		std::string name;
		SectionType type;
		SectionFlag flags;
		Addr address;
		Off offset;
		Xword size;
		Word link;
		Word info;
		Xword addrAlign;
		Xword entSize;
#else
		std::string name;
		SectionType type;
		SectionFlag flags;
		Addr address;
		Off offset;
		Word size;
		Word link;
		Word info;
		Word addrAlign;
		Word entSize;
#endif
	};

	struct Segment
	{
		//NOTE: the offset of 'flags' differs between x64 and i386
		SegmentType type;
		Off offset;
		Addr vaddr;
		Addr paddr;
#if __x86_64__
		Xword fileSize;
		Xword memSize;
		Xword align;
#else
		Word fileSize;
		Word memSize;
		Word align;
#endif
		SegmentFlag flags;
	};

	struct Symbol
	{
#if __x86_64__
		Xword num;
#else
		Word num;
#endif

		std::string name;
		Addr value;
		Word size;
		SymbolType type;
		SymbolBind bind;
		SymbolVisibility visibility;
		ESection index;
	};
private:
	static constexpr void __ProcessSymbolInfo(Symbol& s, unsigned char info, unsigned char other)
	{
#if __x86_64__
		s.bind = ELF64_ST_BIND(info);
		s.type = ELF64_ST_TYPE(info);
		s.visibility = ELF64_ST_VISIBILITY(other);
#else
		s.bind = ELF32_ST_BIND(info);
		s.type = ELF32_ST_TYPE(info);
		s.visibility = ELF32_ST_VISIBILITY(other);
#endif
	}

public:

	static constexpr std::string Demangle(const std::string& mangled)
	{
		if constexpr (Demangler == DemanglerType::GNUCXXABI)
		{
			std::unique_ptr<char, void (*)(void*)> own(abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, nullptr), std::free);
			return own != nullptr ? own.get() : mangled;
		}
		else // default & NONE
		{
			return mangled;
		}
	}

	static constexpr void GetSegments(std::uintptr_t library, std::function<void(Segment&&)> callback)
	{
		Ehdr* ehdr = reinterpret_cast<Ehdr*>(library);
		Shdr* shdr = reinterpret_cast<Shdr*>(library + ehdr->e_shoff);
		Phdr* phdr = reinterpret_cast<Phdr*>(library + ehdr->e_phoff);
		Half num = ehdr->e_shnum;

		for (Half i = 0; i < num; ++i)
		{
			Segment s
			{
				.type     = phdr[i].p_type,
				.offset   = phdr[i].p_offset,
				.vaddr    = phdr[i].p_vaddr,
				.paddr    = phdr[i].p_paddr,
				.fileSize = phdr[i].p_filesz,
				.memSize  = phdr[i].p_memsz,
				.align    = phdr[i].p_align,
				.flags    = phdr[i].p_flags,
			};

			callback(std::move(s));
		}
	}
	
	static constexpr void GetSections(std::uintptr_t library, std::function<void(Section&&)> callback)
	{
		Ehdr* ehdr = reinterpret_cast<Ehdr*>(library);
		Shdr* shdr = reinterpret_cast<Shdr*>((int)library + ehdr->e_shoff);
		Half num = ehdr->e_shnum;

		const char* strTab = reinterpret_cast<char*>(library) + shdr[ehdr->e_shstrndx].sh_offset;

		for (Half i = 0; i < num; ++i)
		{
			Section s
			{
				.index     = i,
				.name      = std::string(strTab + shdr[i].sh_name),
				.type      = shdr[i].sh_type,
				.address   = shdr[i].sh_addr,
				.offset    = shdr[i].sh_offset,
				.size      = shdr[i].sh_size,
				.addrAlign = shdr[i].sh_addralign,
				.entSize   = shdr[i].sh_entsize,
			};

			callback(std::move(s));
		}
	}

	static constexpr void GetSymbols(std::uintptr_t library, std::function<void(Symbol&&)> callback)
	{
		std::deque<Section> secs;
		GetSections(library, [&secs](Section&& s){ secs.push_back(std::move(s)); });

		GetSymbols(library, secs, callback);
	}

	static constexpr void GetSymbols(std::uintptr_t library, const std::deque<Section>& secs, std::function<void(Symbol&&)> callback)
	{
		// Get strTab
		const char* strTab = nullptr;
		for (const auto& sec: secs)
		{
			if (sec.type == SectionType::STRTAB && sec.name == ".strtab"s)
			{
				strTab = (char*)library + sec.offset;
				break;
			}
		}

		// get dynStr
		const char *dynStr = nullptr;
		for(const auto& sec: secs)
		{
			if (sec.type == SectionType::STRTAB && sec.name == ".dynstr"s)
			{
				dynStr = (char*)library + sec.offset;
				break;
			}
		}

		std::vector<Symbol> symbols;
		for(const auto& sec: secs)
		{
			if (sec.type != SectionType::SYMTAB && sec.type != SectionType::DYNSYM)
				continue;

			auto total_syms = sec.size / sizeof(Esym);
			auto syms_data = reinterpret_cast<Esym*>(library + sec.offset);

#if __x86_64__
			for (Xword i = 0; i < total_syms; ++i)
#else
			for (Word i = 0; i < total_syms; ++i)
#endif
			{
				Symbol s;
				s.num   = i;
				s.value = syms_data[i].st_value;
				s.size  = syms_data[i].st_size;
				s.index = syms_data[i].st_shndx;
				__ProcessSymbolInfo(s, syms_data[i].st_info, syms_data[i].st_other);
				
				if(sec.type == SectionType::SYMTAB)
					s.name = Demangle(std::string(strTab + syms_data[i].st_name));
				else if(sec.type == SectionType::DYNSYM)
					s.name = Demangle(std::string(dynStr + syms_data[i].st_name));
				
				callback(std::move(s));
			}
		}
	}

	constexpr static inline std::uintptr_t Resolve(const Symbol& symbol, const std::deque<Segment>& segs, const std::deque<Maps::MapEntry>& module)
	{
		const std::uintptr_t addr = module.front().address + symbol.value - module.front().offset;

		auto m = module.cbegin();
		auto s = segs.cbegin();
		for (; m != module.cend(); ++m, ++s)
		for (const auto& m : module)
		{
			if (m.address > addr - s->align)
				return m.address + symbol.value - m.offset - s->align;
		}

		return module.back().address + symbol.value - module.back().offset - segs.back().align;
	}
};

#endif // UTIL_ELF_HPP
