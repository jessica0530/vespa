// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
#pragma once

#include <boost/signals2/signal.hpp>

#include <vespa/filedistribution/rpc/fileprovider.h>
#include <vespa/filedistribution/model/filedistributionmodel.h>
#include <boost/enable_shared_from_this.hpp>
#include "filedownloader.h"

namespace filedistribution {

class FileDownloaderManager : public FileProvider,
                              public boost::enable_shared_from_this<FileDownloaderManager> {

    class StartDownloads {
        FileDownloaderManager& _parent;
    public:
        void operator()();
        void downloadFile(const std::string& fileReference);
        StartDownloads(FileDownloaderManager* parent);
    };

    class SetFinishedDownloadingStatus {
        FileDownloaderManager& _parent;
      public:
        void operator()(const std::string& fileReference, const boost::filesystem::path&);
        SetFinishedDownloadingStatus(FileDownloaderManager*);
    };

    typedef std::lock_guard<std::mutex> LockGuard;
    std::mutex _updateFilesToDownloadMutex;

    std::shared_ptr<FileDownloader> _fileDownloader;
    std::shared_ptr<FileDistributionModel> _fileDistributionModel;
    StartDownloads _startDownloads;
    SetFinishedDownloadingStatus _setFinishedDownloadingStatus;

    boost::signals2::scoped_connection _downloadFailedConnection;
    boost::signals2::scoped_connection _downloadCompletedConnection;
    boost::signals2::scoped_connection _filesToDownloadChangedConnection;

    void removePeerStatus(const std::string& fileReference);
public:
    FileDownloaderManager(const FileDownloaderManager &) = delete;
    FileDownloaderManager & operator = (const FileDownloaderManager &) = delete;
    FileDownloaderManager(const std::shared_ptr<FileDownloader>&,
            const std::shared_ptr<FileDistributionModel>& model);
    ~FileDownloaderManager();
    void start();

    boost::optional<boost::filesystem::path> getPath(const std::string& fileReference);
    void downloadFile(const std::string& fileReference);

    //FileProvider overrides
    DownloadCompletedSignal& downloadCompleted() {
        return _fileDownloader->_downloadCompleted;
    }

    DownloadFailedSignal& downloadFailed() {
        return _fileDownloader->_downloadFailed;
    }
};

} //namespace filedistribution


