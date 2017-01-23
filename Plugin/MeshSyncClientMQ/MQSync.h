#pragma once

#include "MeshSync/msClient.h"

struct MQSync
{
public:
    MQSync();
    ~MQSync();
    ms::ClientSettings& getClientSettings();
    float& getScaleFactor();
    bool& getAutoSync();
    bool& getBakeSkin();
    bool& getBakeCloth();

    void clear();
    void flushPendingRequests(MQDocument doc);
    void sendMesh(MQDocument doc, bool force = false);
    bool importMeshes(MQDocument doc);

private:
    bool isAsyncSendInProgress();
    void waitAsyncSend();
    MQObject findMQObject(MQDocument doc, const char *name);
    MQObject createObject(const ms::Mesh& data, const char *name);
    void extractMeshData(MQDocument doc, MQObject obj, ms::Mesh& data);
    void copyPointsForNormalCalculation(MQDocument doc, MQObject obj, ms::Mesh& data);

    using ClientMeshes = std::vector<ms::MeshPtr>;
    using HostMeshes = std::map<int, ms::MeshPtr>;
    using ExistRecords = std::map<std::string, bool>;

    struct Relation
    {
        ms::MeshPtr data;
        MQObject obj = nullptr;
        MQObject normal = nullptr;
    };

    ms::ClientSettings m_settings;
    float m_scale_factor = 200.0f;
    bool m_auto_sync = false;

    bool m_bake_skin = false;
    bool m_bake_cloth = false;

    ClientMeshes m_client_meshes;
    HostMeshes m_host_meshes;

    std::vector<MQObject> m_obj_for_normals;
    std::vector<Relation> m_relations;
    ExistRecords m_exist_record;
    std::future<void> m_future_send;
    bool m_pending_send_meshes = false;
};
