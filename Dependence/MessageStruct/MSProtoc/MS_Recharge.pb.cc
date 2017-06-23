// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MSProtoc/MS_Recharge.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MSProtoc/MS_Recharge.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace MS_Recharge {

namespace {

const ::google::protobuf::Descriptor* Admin_Recharge_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Admin_Recharge_reflection_ = NULL;
const ::google::protobuf::Descriptor* Admin_RetRecharge_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Admin_RetRecharge_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_MSProtoc_2fMS_5fRecharge_2eproto() {
  protobuf_AddDesc_MSProtoc_2fMS_5fRecharge_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MSProtoc/MS_Recharge.proto");
  GOOGLE_CHECK(file != NULL);
  Admin_Recharge_descriptor_ = file->message_type(0);
  static const int Admin_Recharge_offsets_[9] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, orderid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, payname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, openid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, money_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, gold_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, charid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, isfirstrecharge_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, source_),
  };
  Admin_Recharge_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Admin_Recharge_descriptor_,
      Admin_Recharge::default_instance_,
      Admin_Recharge_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_Recharge, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Admin_Recharge));
  Admin_RetRecharge_descriptor_ = file->message_type(1);
  static const int Admin_RetRecharge_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_RetRecharge, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_RetRecharge, rechargeinfo_),
  };
  Admin_RetRecharge_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Admin_RetRecharge_descriptor_,
      Admin_RetRecharge::default_instance_,
      Admin_RetRecharge_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_RetRecharge, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Admin_RetRecharge, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Admin_RetRecharge));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MSProtoc_2fMS_5fRecharge_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Admin_Recharge_descriptor_, &Admin_Recharge::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Admin_RetRecharge_descriptor_, &Admin_RetRecharge::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_MSProtoc_2fMS_5fRecharge_2eproto() {
  delete Admin_Recharge::default_instance_;
  delete Admin_Recharge_reflection_;
  delete Admin_RetRecharge::default_instance_;
  delete Admin_RetRecharge_reflection_;
}

void protobuf_AddDesc_MSProtoc_2fMS_5fRecharge_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\032MSProtoc/MS_Recharge.proto\022\013MS_Recharg"
    "e\"\246\001\n\016Admin_Recharge\022\017\n\007orderID\030\001 \002(\t\022\014\n"
    "\004type\030\002 \002(\005\022\017\n\007payname\030\003 \002(\005\022\016\n\006openID\030\004"
    " \002(\t\022\r\n\005money\030\005 \002(\003\022\014\n\004gold\030\006 \002(\005\022\016\n\006cha"
    "rID\030\007 \002(\003\022\027\n\017isFirstRecharge\030\010 \001(\005\022\016\n\006so"
    "urce\030\t \001(\t\"S\n\021Admin_RetRecharge\022\013\n\003ret\030\001"
    " \002(\005\0221\n\014rechargeInfo\030\002 \001(\0132\033.MS_Recharge"
    ".Admin_Recharge", 295);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MSProtoc/MS_Recharge.proto", &protobuf_RegisterTypes);
  Admin_Recharge::default_instance_ = new Admin_Recharge();
  Admin_RetRecharge::default_instance_ = new Admin_RetRecharge();
  Admin_Recharge::default_instance_->InitAsDefaultInstance();
  Admin_RetRecharge::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MSProtoc_2fMS_5fRecharge_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MSProtoc_2fMS_5fRecharge_2eproto {
  StaticDescriptorInitializer_MSProtoc_2fMS_5fRecharge_2eproto() {
    protobuf_AddDesc_MSProtoc_2fMS_5fRecharge_2eproto();
  }
} static_descriptor_initializer_MSProtoc_2fMS_5fRecharge_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Admin_Recharge::kOrderIDFieldNumber;
const int Admin_Recharge::kTypeFieldNumber;
const int Admin_Recharge::kPaynameFieldNumber;
const int Admin_Recharge::kOpenIDFieldNumber;
const int Admin_Recharge::kMoneyFieldNumber;
const int Admin_Recharge::kGoldFieldNumber;
const int Admin_Recharge::kCharIDFieldNumber;
const int Admin_Recharge::kIsFirstRechargeFieldNumber;
const int Admin_Recharge::kSourceFieldNumber;
#endif  // !_MSC_VER

Admin_Recharge::Admin_Recharge()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Admin_Recharge::InitAsDefaultInstance() {
}

Admin_Recharge::Admin_Recharge(const Admin_Recharge& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Admin_Recharge::SharedCtor() {
  _cached_size_ = 0;
  orderid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  type_ = 0;
  payname_ = 0;
  openid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  money_ = GOOGLE_LONGLONG(0);
  gold_ = 0;
  charid_ = GOOGLE_LONGLONG(0);
  isfirstrecharge_ = 0;
  source_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Admin_Recharge::~Admin_Recharge() {
  SharedDtor();
}

void Admin_Recharge::SharedDtor() {
  if (orderid_ != &::google::protobuf::internal::kEmptyString) {
    delete orderid_;
  }
  if (openid_ != &::google::protobuf::internal::kEmptyString) {
    delete openid_;
  }
  if (source_ != &::google::protobuf::internal::kEmptyString) {
    delete source_;
  }
  if (this != default_instance_) {
  }
}

void Admin_Recharge::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Admin_Recharge::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Admin_Recharge_descriptor_;
}

const Admin_Recharge& Admin_Recharge::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MSProtoc_2fMS_5fRecharge_2eproto();
  return *default_instance_;
}

Admin_Recharge* Admin_Recharge::default_instance_ = NULL;

Admin_Recharge* Admin_Recharge::New() const {
  return new Admin_Recharge;
}

void Admin_Recharge::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_orderid()) {
      if (orderid_ != &::google::protobuf::internal::kEmptyString) {
        orderid_->clear();
      }
    }
    type_ = 0;
    payname_ = 0;
    if (has_openid()) {
      if (openid_ != &::google::protobuf::internal::kEmptyString) {
        openid_->clear();
      }
    }
    money_ = GOOGLE_LONGLONG(0);
    gold_ = 0;
    charid_ = GOOGLE_LONGLONG(0);
    isfirstrecharge_ = 0;
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (has_source()) {
      if (source_ != &::google::protobuf::internal::kEmptyString) {
        source_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Admin_Recharge::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string orderID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_orderid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->orderid().data(), this->orderid().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_type;
        break;
      }

      // required int32 type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_payname;
        break;
      }

      // required int32 payname = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_payname:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &payname_)));
          set_has_payname();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_openID;
        break;
      }

      // required string openID = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_openID:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_openid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->openid().data(), this->openid().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_money;
        break;
      }

      // required int64 money = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_money:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &money_)));
          set_has_money();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_gold;
        break;
      }

      // required int32 gold = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_gold:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &gold_)));
          set_has_gold();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_charID;
        break;
      }

      // required int64 charID = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_charID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &charid_)));
          set_has_charid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(64)) goto parse_isFirstRecharge;
        break;
      }

      // optional int32 isFirstRecharge = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_isFirstRecharge:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &isfirstrecharge_)));
          set_has_isfirstrecharge();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(74)) goto parse_source;
        break;
      }

      // optional string source = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_source:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_source()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->source().data(), this->source().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Admin_Recharge::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string orderID = 1;
  if (has_orderid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->orderid().data(), this->orderid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->orderid(), output);
  }

  // required int32 type = 2;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->type(), output);
  }

  // required int32 payname = 3;
  if (has_payname()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->payname(), output);
  }

  // required string openID = 4;
  if (has_openid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openid().data(), this->openid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->openid(), output);
  }

  // required int64 money = 5;
  if (has_money()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(5, this->money(), output);
  }

  // required int32 gold = 6;
  if (has_gold()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(6, this->gold(), output);
  }

  // required int64 charID = 7;
  if (has_charid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(7, this->charid(), output);
  }

  // optional int32 isFirstRecharge = 8;
  if (has_isfirstrecharge()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(8, this->isfirstrecharge(), output);
  }

  // optional string source = 9;
  if (has_source()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->source().data(), this->source().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      9, this->source(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Admin_Recharge::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string orderID = 1;
  if (has_orderid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->orderid().data(), this->orderid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->orderid(), target);
  }

  // required int32 type = 2;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->type(), target);
  }

  // required int32 payname = 3;
  if (has_payname()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->payname(), target);
  }

  // required string openID = 4;
  if (has_openid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->openid().data(), this->openid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->openid(), target);
  }

  // required int64 money = 5;
  if (has_money()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(5, this->money(), target);
  }

  // required int32 gold = 6;
  if (has_gold()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(6, this->gold(), target);
  }

  // required int64 charID = 7;
  if (has_charid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(7, this->charid(), target);
  }

  // optional int32 isFirstRecharge = 8;
  if (has_isfirstrecharge()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(8, this->isfirstrecharge(), target);
  }

  // optional string source = 9;
  if (has_source()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->source().data(), this->source().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        9, this->source(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Admin_Recharge::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string orderID = 1;
    if (has_orderid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->orderid());
    }

    // required int32 type = 2;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
    }

    // required int32 payname = 3;
    if (has_payname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->payname());
    }

    // required string openID = 4;
    if (has_openid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->openid());
    }

    // required int64 money = 5;
    if (has_money()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->money());
    }

    // required int32 gold = 6;
    if (has_gold()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->gold());
    }

    // required int64 charID = 7;
    if (has_charid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->charid());
    }

    // optional int32 isFirstRecharge = 8;
    if (has_isfirstrecharge()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->isfirstrecharge());
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional string source = 9;
    if (has_source()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->source());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Admin_Recharge::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Admin_Recharge* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Admin_Recharge*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Admin_Recharge::MergeFrom(const Admin_Recharge& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_orderid()) {
      set_orderid(from.orderid());
    }
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_payname()) {
      set_payname(from.payname());
    }
    if (from.has_openid()) {
      set_openid(from.openid());
    }
    if (from.has_money()) {
      set_money(from.money());
    }
    if (from.has_gold()) {
      set_gold(from.gold());
    }
    if (from.has_charid()) {
      set_charid(from.charid());
    }
    if (from.has_isfirstrecharge()) {
      set_isfirstrecharge(from.isfirstrecharge());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_source()) {
      set_source(from.source());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Admin_Recharge::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Admin_Recharge::CopyFrom(const Admin_Recharge& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Admin_Recharge::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000007f) != 0x0000007f) return false;

  return true;
}

void Admin_Recharge::Swap(Admin_Recharge* other) {
  if (other != this) {
    std::swap(orderid_, other->orderid_);
    std::swap(type_, other->type_);
    std::swap(payname_, other->payname_);
    std::swap(openid_, other->openid_);
    std::swap(money_, other->money_);
    std::swap(gold_, other->gold_);
    std::swap(charid_, other->charid_);
    std::swap(isfirstrecharge_, other->isfirstrecharge_);
    std::swap(source_, other->source_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Admin_Recharge::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Admin_Recharge_descriptor_;
  metadata.reflection = Admin_Recharge_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Admin_RetRecharge::kRetFieldNumber;
const int Admin_RetRecharge::kRechargeInfoFieldNumber;
#endif  // !_MSC_VER

Admin_RetRecharge::Admin_RetRecharge()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Admin_RetRecharge::InitAsDefaultInstance() {
  rechargeinfo_ = const_cast< ::MS_Recharge::Admin_Recharge*>(&::MS_Recharge::Admin_Recharge::default_instance());
}

Admin_RetRecharge::Admin_RetRecharge(const Admin_RetRecharge& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Admin_RetRecharge::SharedCtor() {
  _cached_size_ = 0;
  ret_ = 0;
  rechargeinfo_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Admin_RetRecharge::~Admin_RetRecharge() {
  SharedDtor();
}

void Admin_RetRecharge::SharedDtor() {
  if (this != default_instance_) {
    delete rechargeinfo_;
  }
}

void Admin_RetRecharge::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Admin_RetRecharge::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Admin_RetRecharge_descriptor_;
}

const Admin_RetRecharge& Admin_RetRecharge::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MSProtoc_2fMS_5fRecharge_2eproto();
  return *default_instance_;
}

Admin_RetRecharge* Admin_RetRecharge::default_instance_ = NULL;

Admin_RetRecharge* Admin_RetRecharge::New() const {
  return new Admin_RetRecharge;
}

void Admin_RetRecharge::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ret_ = 0;
    if (has_rechargeinfo()) {
      if (rechargeinfo_ != NULL) rechargeinfo_->::MS_Recharge::Admin_Recharge::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Admin_RetRecharge::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ret_)));
          set_has_ret();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_rechargeInfo;
        break;
      }

      // optional .MS_Recharge.Admin_Recharge rechargeInfo = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_rechargeInfo:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_rechargeinfo()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Admin_RetRecharge::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->ret(), output);
  }

  // optional .MS_Recharge.Admin_Recharge rechargeInfo = 2;
  if (has_rechargeinfo()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->rechargeinfo(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Admin_RetRecharge::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->ret(), target);
  }

  // optional .MS_Recharge.Admin_Recharge rechargeInfo = 2;
  if (has_rechargeinfo()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->rechargeinfo(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Admin_RetRecharge::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ret());
    }

    // optional .MS_Recharge.Admin_Recharge rechargeInfo = 2;
    if (has_rechargeinfo()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->rechargeinfo());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Admin_RetRecharge::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Admin_RetRecharge* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Admin_RetRecharge*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Admin_RetRecharge::MergeFrom(const Admin_RetRecharge& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
    if (from.has_rechargeinfo()) {
      mutable_rechargeinfo()->::MS_Recharge::Admin_Recharge::MergeFrom(from.rechargeinfo());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Admin_RetRecharge::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Admin_RetRecharge::CopyFrom(const Admin_RetRecharge& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Admin_RetRecharge::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_rechargeinfo()) {
    if (!this->rechargeinfo().IsInitialized()) return false;
  }
  return true;
}

void Admin_RetRecharge::Swap(Admin_RetRecharge* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(rechargeinfo_, other->rechargeinfo_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Admin_RetRecharge::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Admin_RetRecharge_descriptor_;
  metadata.reflection = Admin_RetRecharge_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MS_Recharge

// @@protoc_insertion_point(global_scope)
