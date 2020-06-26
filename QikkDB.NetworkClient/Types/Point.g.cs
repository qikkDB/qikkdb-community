// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: Types/Point.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace QikkDB.Types {

  /// <summary>Holder for reflection information generated from Types/Point.proto</summary>
  public static partial class PointReflection {

    #region Descriptor
    /// <summary>File descriptor for Types/Point.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static PointReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "ChFUeXBlcy9Qb2ludC5wcm90bxIMUWlra0RCLlR5cGVzGhpUeXBlcy9Db21w",
            "bGV4UG9seWdvbi5wcm90byIxCgVQb2ludBIoCghnZW9Qb2ludBgBIAEoCzIW",
            "LlFpa2tEQi5UeXBlcy5HZW9Qb2ludGIGcHJvdG8z"));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { global::QikkDB.Types.ComplexPolygonReflection.Descriptor, },
          new pbr::GeneratedClrTypeInfo(null, null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::QikkDB.Types.Point), global::QikkDB.Types.Point.Parser, new[]{ "GeoPoint" }, null, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class Point : pb::IMessage<Point> {
    private static readonly pb::MessageParser<Point> _parser = new pb::MessageParser<Point>(() => new Point());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Point> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::QikkDB.Types.PointReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point(Point other) : this() {
      geoPoint_ = other.geoPoint_ != null ? other.geoPoint_.Clone() : null;
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Point Clone() {
      return new Point(this);
    }

    /// <summary>Field number for the "geoPoint" field.</summary>
    public const int GeoPointFieldNumber = 1;
    private global::QikkDB.Types.GeoPoint geoPoint_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::QikkDB.Types.GeoPoint GeoPoint {
      get { return geoPoint_; }
      set {
        geoPoint_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Point);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Point other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(GeoPoint, other.GeoPoint)) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (geoPoint_ != null) hash ^= GeoPoint.GetHashCode();
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (geoPoint_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(GeoPoint);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (geoPoint_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(GeoPoint);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Point other) {
      if (other == null) {
        return;
      }
      if (other.geoPoint_ != null) {
        if (geoPoint_ == null) {
          GeoPoint = new global::QikkDB.Types.GeoPoint();
        }
        GeoPoint.MergeFrom(other.GeoPoint);
      }
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 10: {
            if (geoPoint_ == null) {
              GeoPoint = new global::QikkDB.Types.GeoPoint();
            }
            input.ReadMessage(GeoPoint);
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
