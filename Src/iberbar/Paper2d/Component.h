#pragma once


#include <iberbar/Paper2d/Headers.h>

namespace iberbar
{
	namespace Paper2d
	{
		class CNode;


		enum class UComponentType : uint32
		{
			CustomAttributes = 1,
			Updatable = 2,
			HandleMouseInput = 3,
			HandleKeyboardInput = 4,
			Transform = 5,
			Bounding = 6,

			Custom = 0xffffffff
		};


		class __iberbarPaper2dApis__ CComponent abstract
			: public CRef
		{
		public:
			CComponent( UComponentType nComponentType );
			virtual ~CComponent();

		protected:
			virtual void OnAttach();
			virtual void OnRemove();

		public:
			void SetOwner( CNode* pNode );
			void SetName( const char* strName ) { m_strName = strName; }
			bool WillRemoveFromSystem() const { return m_bRemoveFromSystem; }
			CNode* GetOwner() const { return m_pOwner; }
			const std::string& GetName() const { return m_strName; }
			bool IsComponentType( UComponentType nType ) const { return m_nComponentType == nType; }
			void SetEnable( bool bEnable );
			bool GetEnable() const { return m_bEnable; }
			bool IsEnable() const;

		private:
			UComponentType m_nComponentType;
			CNode* m_pOwner;
			std::string m_strName;
		protected:
			bool m_bEnable;
			bool m_bRemoveFromSystem;
		};
	}
}



