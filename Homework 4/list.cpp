#include <iostream>
#include <string>
#include <vector>

using namespace std;

class File
{
public:
    File(string nm) : m_name(nm) {}
    virtual ~File() {}
    string name() const { return m_name; }
    virtual bool add(File* f) = 0;
    virtual const vector<File*>* files() const = 0;
private:
    string m_name;
};

class PlainFile : public File
{
public:
    PlainFile(string nm) : File(nm) {}
    virtual bool add(File* f) { return false; }
    virtual const vector<File*>* files() const { return nullptr; }
};

class Directory : public File
{
public:
    Directory(string nm) : File(nm) {}
    virtual ~Directory();
    virtual bool add(File* f) { m_files.push_back(f); return true; }
    virtual const vector<File*>* files() const { return &m_files; }
private:
    vector<File*> m_files;
};

Directory::~Directory()
{
    for (int k = 0; k < m_files.size(); k++)
        delete m_files[k];
}

void listAll(string path, const File* f)  // two-parameter overload
{
    if (f == nullptr) {
        return;
    }
    bool atPlainFile = (f->files() == nullptr);
    if (atPlainFile) { // At a plain file, don't add slash
        path += f->name();
    }
    else if (path != f->name()) { // In a directory that isn't the root, add directory name and slash
        path += f->name() + '/'; 
    }
    else { // Already in the root directory, only add a slash
        path += '/'; 
    }
    cout << path << endl;
    if (atPlainFile) {
        return;
    }
    for (int i = 0; i < f->files()->size(); i++) {
        listAll(path, f->files()->at(i));
    }
}

void listAll(const File* f)  // one-parameter overload
{
    if (f != nullptr)
        listAll(f->name(), f);
}

int main()
{
    Directory* d1 = new Directory("Fun");
    d1->add(new PlainFile("party.jpg"));
    d1->add(new PlainFile("beach.jpg"));
    d1->add(new PlainFile("skitrip.jpg"));
    Directory* d2 = new Directory("Work");
    d2->add(new PlainFile("seaslab.jpg"));
    Directory* d3 = new Directory("My Pictures");
    d3->add(d1);
    d3->add(new PlainFile("me.jpg"));
    d3->add(new Directory("Miscellaneous"));
    d3->add(d2);
    listAll(d3);
    delete d3;
}