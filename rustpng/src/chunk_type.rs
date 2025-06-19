use core::fmt;
use std::fmt::Display;
use std::str::FromStr;

#[derive(Eq,PartialEq,Debug)]
struct ChunkType {
    ancillary: u8,    // 大写关键块,小写辅助块
    private: u8,      //大写公共,小写私有
    reserved: u8,     // 必须大写
    safe_to_copy: u8, //大写不安全复制,小写安全复制
}

/// convert a [u8;4] to ChunkType,check the validity
impl TryFrom<[u8; 4]> for ChunkType {
    type Error = &'static str;
    fn try_from(value: [u8; 4]) -> Result<Self, Self::Error> {
        if value[2].is_ascii_lowercase() {
            return Err("The third type must be a Uppercase Alphabet");
        } else {
            Ok(ChunkType {
                ancillary: value[0],
                private: value[1],
                reserved: value[2],
                safe_to_copy: value[3],
            })
        }
    }
}

/// convert a str to ChunkType,check the validity
impl FromStr for ChunkType {
    type Err = &'static str;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s.len() > 4 {
            return Err("Str's length should be 4!");
        }
        let bytes = s.as_bytes();
        if bytes[2].is_ascii_lowercase() {
            return Err("The third type must be a Uppercase Alphabet");
        } else {
            Ok(ChunkType {
                ancillary: bytes[0],
                private: bytes[1],
                reserved: bytes[2],
                safe_to_copy: bytes[3],
            })
        }
    }
}

impl Display for ChunkType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "
    Chunk:{},
    ancillary:{}-{},
    private:{}-{},
    reserved:{}-{},
    safe_to_copy:{}-{}
    ",
            self.to_string(),
            self.ancillary as char,
            self.ancillary,
            self.private as char,
            self.private,
            self.reserved as char,
            self.reserved,
            self.safe_to_copy as char,
            self.safe_to_copy
        )
    }
}



impl ChunkType {
    fn to_string(&self) -> String {
        let bytes = &[
            self.ancillary,
            self.private,
            self.reserved,
            self.safe_to_copy,
        ];
        bytes.iter().map(|&b| b as char).collect()
    }

    fn bytes(&self)->[u8;4]{
      [self.ancillary,self.private,self.reserved,self.safe_to_copy]
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::convert::TryFrom;
    use std::str::FromStr;

    #[test]
    pub fn test_chunk_type_from_bytes() {
        let expected = [82, 117, 83, 116];
        let actual = ChunkType::try_from([82, 117, 83, 116]).unwrap();

        assert_eq!(expected, actual.bytes());
    }

    #[test]
    pub fn test_chunk_type_from_str() {
        let expected = ChunkType::try_from([82, 117, 83, 116]).unwrap();
        let actual = ChunkType::from_str("RuSt").unwrap();
        assert_eq!(expected, actual);
    }

    #[test]
    pub fn test_chunk_type_is_critical() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert!(chunk.is_critical());
    }

    #[test]
    pub fn test_chunk_type_is_not_critical() {
        let chunk = ChunkType::from_str("ruSt").unwrap();
        assert!(!chunk.is_critical());
    }

    #[test]
    pub fn test_chunk_type_is_public() {
        let chunk = ChunkType::from_str("RUSt").unwrap();
        assert!(chunk.is_public());
    }

    #[test]
    pub fn test_chunk_type_is_not_public() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert!(!chunk.is_public());
    }

    #[test]
    pub fn test_chunk_type_is_reserved_bit_valid() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert!(chunk.is_reserved_bit_valid());
    }

    #[test]
    pub fn test_chunk_type_is_reserved_bit_invalid() {
        let chunk = ChunkType::from_str("Rust").unwrap();
        assert!(!chunk.is_reserved_bit_valid());
    }

    #[test]
    pub fn test_chunk_type_is_safe_to_copy() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert!(chunk.is_safe_to_copy());
    }

    #[test]
    pub fn test_chunk_type_is_unsafe_to_copy() {
        let chunk = ChunkType::from_str("RuST").unwrap();
        assert!(!chunk.is_safe_to_copy());
    }

    #[test]
    pub fn test_valid_chunk_is_valid() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert!(chunk.is_valid());
    }

    #[test]
    pub fn test_invalid_chunk_is_valid() {
        let chunk = ChunkType::from_str("Rust").unwrap();
        assert!(!chunk.is_valid());

        let chunk = ChunkType::from_str("Ru1t");
        assert!(chunk.is_err());
    }

    #[test]
    pub fn test_chunk_type_string() {
        let chunk = ChunkType::from_str("RuSt").unwrap();
        assert_eq!(&chunk.to_string(), "RuSt");
    }

    #[test]
    pub fn test_chunk_type_trait_impls() {
        let chunk_type_1: ChunkType = TryFrom::try_from([82, 117, 83, 116]).unwrap();
        let chunk_type_2: ChunkType = FromStr::from_str("RuSt").unwrap();
        let _chunk_string = format!("{}", chunk_type_1);
        let _are_chunks_equal = chunk_type_1 == chunk_type_2;
    }
}
