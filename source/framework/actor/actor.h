/**
 * @file actor.h
 * @brief �Q�[���I�u�W�F�N�g�̐U�镑��������
 */

#pragma once

#include"../component/component.h"

 /**
 * @brief �Q�[���I�u�W�F�N�g�̐U�镑��������N���X
 */
class Actor
{
public:
	enum ActorType
	{
		ACTORTYPE_NONE,
		ACTORTYPE_PLAYER,
		ACTORTYPE_ENEMY,
		ACTORTYPE_ALLIES,
		ACTORTYPE_PLAYER_BULLET,
		ACTORTYPE_ENEMY_BULLET,
		ACTORTYPE_BEAMRIFLE,
		ACTORTYPE_CANNON,
		ACTORTYPE_BEAMSABER_NKAKU001,
		ACTORTYPE_BEAMSABER_NKAKU002,
		ACTORTYPE_BEAMSABER_NKAKU003,
		ACTORTYPE_MAX,
	};

protected:
	//! �A�N�^�[�̃^�C�v
	ActorType m_ActorType;

	//! �A�N�e�B�u���ǂ���
	bool m_Active;

	// ���W�ϊ�
	//! ���[���h�s��
	Conv_XM::Matrix4x4f m_WorldMatrix;
	//! �|�W�V����
	Conv_XM::Vector3f m_Position;
	//! �X�P�[��
	Conv_XM::Vector3f m_Scale;
	//! �N�H�[�^�j�I��
	Conv_XM::Vector4f m_Quaternion;
	//! ���W�ϊ����s���邩�ǂ����̃t���O
	bool m_RecomputeTransform;

	//! �A�N�^�[�����R���|�[�l���g
	std::vector<class Component*> m_Components;
public:
	Actor();
	virtual ~Actor();

	/**
	* @brief ����������
	* @details �I�u�W�F�N�g�}�l�[�W���[����Ăяo�����ǂݍ��ݏ����֐�(�I�[�o�[���C�h�s��)
	*/
	void LoadData();

	/**
	* @brief ����������
	* @details �I�u�W�F�N�g�}�l�[�W���[����Ăяo����鏉���������֐�(�I�[�o�[���C�h�s��)
	*/
	void Init();

	/**
	* @brief �I������
	* @details �I�u�W�F�N�g�}�l�[�W���[����Ăяo�����I�������֐�(�I�[�o�[���C�h�s��)
	*/
	void Uninit();

	/**
	* @brief ���͏���
	* @details �I�u�W�F�N�g�}�l�[�W���[����Ăяo�������͏����֐�(�I�[�o�[���C�h�s��)
	*/
	void ProcessInput();

	/**
	* @brief �X�V����
	* @details �I�u�W�F�N�g�}�l�[�W���[����Ăяo�����X�V�����֐�(�I�[�o�[���C�h�s��)
	*/
	void Update();

	/**
	* @brief ���[���h�s��X�V�֐�
	* @details �|�W�V�����A�X�P�[���A�N�H�[�^�j�I�����烏�[���h�s����쐬����
	*/
	void ComputeWorldTransform();

	/**
	* @brief �R���|�[�l���g�����֐�
	* @return T �쐬�����R���|�[�l���g
	*/
	template<typename T>
	T* CreateComponent()
	{
		// �R���|�[�l���g�𐶐�
		T* component = new T(this);
		// null�`�F�b�N
		if (component == nullptr)
		{
			Logger::GetInstance().SetLog("Actor::CreateComponent component��nullptr");
			return nullptr;
		}
		// �R���|�[�l���g��ǉ�
		AddComponent(component);

		return component;
	}

	/**
	* @brief �R���|�[�l���g�擾�֐�
	* @return T �e���v���[�g�Ŏw�肳�ꂽ�R���|�[�l���g�̃|�C���^
	* @details �w�肵���R���|�[�l���g������������Ă���ꍇ�̂ݐ���쓮
	*/
	template <typename T>
	T*  GetComponent() const
	{
		for (auto comp : m_Components)
		{
			if (typeid(*comp) == typeid(T))
			{
				T* t = dynamic_cast<T*>(comp);
				return  t;
			}
		}
		return nullptr;
	}

	/**
	* @brief �����蔻�菈��
	* @details �Փˎ��ɌĂяo�����֐�
	*/
	virtual void CollisionDetected(class Actor* coleObj);

	/**
	* @brief �p�x����
	* @param[in] float(x) �p�xX
	* @param[in] float(y) �p�xY
	* @param[in] float(z) �p�xZ
	* @details degree�œ���
	*/
	void SetEuler(float x, float y, float z);

	/**
	* @brief �p�x����
	* @param[in] const Conv_XM::Vector3f&(euler) �p�x
	* @details degree�œ���
	*/
	void SetEuler(const Conv_XM::Vector3f& euler);

	// �Z�b�^�[,�Q�b�^�[

	void SetActorType(ActorType type) { m_ActorType = type; }
	inline ActorType GetActorType() const { return m_ActorType; }

	virtual void SetActive(bool flag) { m_Active = flag; }
	inline bool GetActive() const { return m_Active; }

	inline const Conv_XM::Matrix4x4f& GetWorldMatrix() const { return m_WorldMatrix; }

	inline void SetPosition(const Conv_XM::Vector3f& position)
	{
		m_RecomputeTransform = true;
		m_Position = position; 
	}
	inline const Conv_XM::Vector3f& GetPosition() const { return m_Position; }

	inline void SetScale(const Conv_XM::Vector3f& scale) 
	{
		m_RecomputeTransform = true;
		m_Scale = scale; 
	}
	inline const Conv_XM::Vector3f& GetScale() const { return m_Scale; }

	inline void SetQuaternion(const Conv_XM::Vector4f& quaternion) 
	{
		m_RecomputeTransform = true;
		m_Quaternion = quaternion; 
	}
	inline const Conv_XM::Vector4f& GetQuaternion() const { return m_Quaternion; }

	Conv_XM::Vector3f GetForward() const;
	Conv_XM::Vector3f GetRight() const;
	Conv_XM::Vector3f GetUp() const;
private:

	/**
	* @brief �A�N�^�[�Ǝ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor();

	/**
	* @brief �A�N�^�[�Ǝ��̏���������
	*/
	virtual void InitActor();

	/**
	* @brief �A�N�^�[�Ǝ��̏I������
	*/
	virtual void UninitActor();

	/**
	* @brief �A�N�^�[�Ǝ��̓��͏���
	*/
	virtual void ProcessInputActor();

	/**
	* @brief �A�N�^�[�Ǝ��̍X�V����
	*/
	virtual void UpdateActor();

	/**
	* @brief �A�N�^�[�����S�R���|�[�l���g��ǂݍ��ݏ���(�I�[�o�[���C�h�s��)
	*/
	void LoadComponents();

	/**
	* @brief �A�N�^�[�����S�R���|�[�l���g����������(�I�[�o�[���C�h�s��)
	*/
	void InitComponents();

	/**
	* @brief �A�N�^�[�����S�R���|�[�l���g���I������(�I�[�o�[���C�h�s��)
	*/
	void UninitComponents();

	/**
	* @brief �A�N�^�[�����S�R���|�[�l���g����͏���(�I�[�o�[���C�h�s��)
	*/
	void ProcessInputComponents();

	/**
	* @brief �A�N�^�[�����S�R���|�[�l���g���X�V����(�I�[�o�[���C�h�s��)
	*/
	void UpdateComponents();


	/**
	* @brief �R���|�[�l���g�̓o�^
	* @param[in] class Component*(component) �R���|�[�l���g
	*/
	void AddComponent(Component * component);
};