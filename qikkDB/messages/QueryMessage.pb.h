// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages/QueryMessage.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_messages_2fQueryMessage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_messages_2fQueryMessage_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_messages_2fQueryMessage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_messages_2fQueryMessage_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_messages_2fQueryMessage_2eproto;
namespace QikkDB {
namespace NetworkClient {
namespace Message {
class QueryMessage;
class QueryMessageDefaultTypeInternal;
extern QueryMessageDefaultTypeInternal _QueryMessage_default_instance_;
}  // namespace Message
}  // namespace NetworkClient
}  // namespace QikkDB
PROTOBUF_NAMESPACE_OPEN
template<> ::QikkDB::NetworkClient::Message::QueryMessage* Arena::CreateMaybeMessage<::QikkDB::NetworkClient::Message::QueryMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace QikkDB {
namespace NetworkClient {
namespace Message {

// ===================================================================

class QueryMessage :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:QikkDB.NetworkClient.Message.QueryMessage) */ {
 public:
  QueryMessage();
  virtual ~QueryMessage();

  QueryMessage(const QueryMessage& from);
  QueryMessage(QueryMessage&& from) noexcept
    : QueryMessage() {
    *this = ::std::move(from);
  }

  inline QueryMessage& operator=(const QueryMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline QueryMessage& operator=(QueryMessage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const QueryMessage& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const QueryMessage* internal_default_instance() {
    return reinterpret_cast<const QueryMessage*>(
               &_QueryMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(QueryMessage& a, QueryMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(QueryMessage* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline QueryMessage* New() const final {
    return CreateMaybeMessage<QueryMessage>(nullptr);
  }

  QueryMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<QueryMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const QueryMessage& from);
  void MergeFrom(const QueryMessage& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(QueryMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "QikkDB.NetworkClient.Message.QueryMessage";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_messages_2fQueryMessage_2eproto);
    return ::descriptor_table_messages_2fQueryMessage_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kQueryFieldNumber = 1,
  };
  // string Query = 1;
  void clear_query();
  const std::string& query() const;
  void set_query(const std::string& value);
  void set_query(std::string&& value);
  void set_query(const char* value);
  void set_query(const char* value, size_t size);
  std::string* mutable_query();
  std::string* release_query();
  void set_allocated_query(std::string* query);
  private:
  const std::string& _internal_query() const;
  void _internal_set_query(const std::string& value);
  std::string* _internal_mutable_query();
  public:

  // @@protoc_insertion_point(class_scope:QikkDB.NetworkClient.Message.QueryMessage)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr query_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_messages_2fQueryMessage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// QueryMessage

// string Query = 1;
inline void QueryMessage::clear_query() {
  query_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& QueryMessage::query() const {
  // @@protoc_insertion_point(field_get:QikkDB.NetworkClient.Message.QueryMessage.Query)
  return _internal_query();
}
inline void QueryMessage::set_query(const std::string& value) {
  _internal_set_query(value);
  // @@protoc_insertion_point(field_set:QikkDB.NetworkClient.Message.QueryMessage.Query)
}
inline std::string* QueryMessage::mutable_query() {
  // @@protoc_insertion_point(field_mutable:QikkDB.NetworkClient.Message.QueryMessage.Query)
  return _internal_mutable_query();
}
inline const std::string& QueryMessage::_internal_query() const {
  return query_.GetNoArena();
}
inline void QueryMessage::_internal_set_query(const std::string& value) {
  
  query_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void QueryMessage::set_query(std::string&& value) {
  
  query_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:QikkDB.NetworkClient.Message.QueryMessage.Query)
}
inline void QueryMessage::set_query(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  query_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:QikkDB.NetworkClient.Message.QueryMessage.Query)
}
inline void QueryMessage::set_query(const char* value, size_t size) {
  
  query_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:QikkDB.NetworkClient.Message.QueryMessage.Query)
}
inline std::string* QueryMessage::_internal_mutable_query() {
  
  return query_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* QueryMessage::release_query() {
  // @@protoc_insertion_point(field_release:QikkDB.NetworkClient.Message.QueryMessage.Query)
  
  return query_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void QueryMessage::set_allocated_query(std::string* query) {
  if (query != nullptr) {
    
  } else {
    
  }
  query_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), query);
  // @@protoc_insertion_point(field_set_allocated:QikkDB.NetworkClient.Message.QueryMessage.Query)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Message
}  // namespace NetworkClient
}  // namespace QikkDB

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_messages_2fQueryMessage_2eproto
