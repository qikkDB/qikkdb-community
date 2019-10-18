// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Types/Point.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Types_2fPoint_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Types_2fPoint_2eproto

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
#include "Types/ComplexPolygon.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Types_2fPoint_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Types_2fPoint_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Types_2fPoint_2eproto;
namespace ColmnarDB {
namespace Types {
class Point;
class PointDefaultTypeInternal;
extern PointDefaultTypeInternal _Point_default_instance_;
}  // namespace Types
}  // namespace ColmnarDB
PROTOBUF_NAMESPACE_OPEN
template<> ::ColmnarDB::Types::Point* Arena::CreateMaybeMessage<::ColmnarDB::Types::Point>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace ColmnarDB {
namespace Types {

// ===================================================================

class Point :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ColmnarDB.Types.Point) */ {
 public:
  Point();
  virtual ~Point();

  Point(const Point& from);
  Point(Point&& from) noexcept
    : Point() {
    *this = ::std::move(from);
  }

  inline Point& operator=(const Point& from) {
    CopyFrom(from);
    return *this;
  }
  inline Point& operator=(Point&& from) noexcept {
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
  static const Point& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Point* internal_default_instance() {
    return reinterpret_cast<const Point*>(
               &_Point_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Point& a, Point& b) {
    a.Swap(&b);
  }
  inline void Swap(Point* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Point* New() const final {
    return CreateMaybeMessage<Point>(nullptr);
  }

  Point* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Point>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Point& from);
  void MergeFrom(const Point& from);
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
  void InternalSwap(Point* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ColmnarDB.Types.Point";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Types_2fPoint_2eproto);
    return ::descriptor_table_Types_2fPoint_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGeoPointFieldNumber = 1,
  };
  // .ColmnarDB.Types.GeoPoint geoPoint = 1;
  bool has_geopoint() const;
  private:
  bool _internal_has_geopoint() const;
  public:
  void clear_geopoint();
  const ::ColmnarDB::Types::GeoPoint& geopoint() const;
  ::ColmnarDB::Types::GeoPoint* release_geopoint();
  ::ColmnarDB::Types::GeoPoint* mutable_geopoint();
  void set_allocated_geopoint(::ColmnarDB::Types::GeoPoint* geopoint);
  private:
  const ::ColmnarDB::Types::GeoPoint& _internal_geopoint() const;
  ::ColmnarDB::Types::GeoPoint* _internal_mutable_geopoint();
  public:

  // @@protoc_insertion_point(class_scope:ColmnarDB.Types.Point)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::ColmnarDB::Types::GeoPoint* geopoint_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Types_2fPoint_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Point

// .ColmnarDB.Types.GeoPoint geoPoint = 1;
inline bool Point::_internal_has_geopoint() const {
  return this != internal_default_instance() && geopoint_ != nullptr;
}
inline bool Point::has_geopoint() const {
  return _internal_has_geopoint();
}
inline const ::ColmnarDB::Types::GeoPoint& Point::_internal_geopoint() const {
  const ::ColmnarDB::Types::GeoPoint* p = geopoint_;
  return p != nullptr ? *p : *reinterpret_cast<const ::ColmnarDB::Types::GeoPoint*>(
      &::ColmnarDB::Types::_GeoPoint_default_instance_);
}
inline const ::ColmnarDB::Types::GeoPoint& Point::geopoint() const {
  // @@protoc_insertion_point(field_get:ColmnarDB.Types.Point.geoPoint)
  return _internal_geopoint();
}
inline ::ColmnarDB::Types::GeoPoint* Point::release_geopoint() {
  // @@protoc_insertion_point(field_release:ColmnarDB.Types.Point.geoPoint)
  
  ::ColmnarDB::Types::GeoPoint* temp = geopoint_;
  geopoint_ = nullptr;
  return temp;
}
inline ::ColmnarDB::Types::GeoPoint* Point::_internal_mutable_geopoint() {
  
  if (geopoint_ == nullptr) {
    auto* p = CreateMaybeMessage<::ColmnarDB::Types::GeoPoint>(GetArenaNoVirtual());
    geopoint_ = p;
  }
  return geopoint_;
}
inline ::ColmnarDB::Types::GeoPoint* Point::mutable_geopoint() {
  // @@protoc_insertion_point(field_mutable:ColmnarDB.Types.Point.geoPoint)
  return _internal_mutable_geopoint();
}
inline void Point::set_allocated_geopoint(::ColmnarDB::Types::GeoPoint* geopoint) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(geopoint_);
  }
  if (geopoint) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      geopoint = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, geopoint, submessage_arena);
    }
    
  } else {
    
  }
  geopoint_ = geopoint;
  // @@protoc_insertion_point(field_set_allocated:ColmnarDB.Types.Point.geoPoint)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Types
}  // namespace ColmnarDB

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Types_2fPoint_2eproto
